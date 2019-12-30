//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "nbt/nbt.h"

#ifdef __APPLE__
#define I_HATE_CANCER
#else // Sane people
#undef I_HATE_CANCER
#endif

extern "C" const unsigned char TEXTURE_1_8_textures_0_png[];
extern "C" const size_t TEXTURE_1_8_textures_0_png_len;

extern "C" const unsigned char SHADER_chunk_frag[];
extern "C" const size_t SHADER_chunk_frag_len;

extern "C" const unsigned char SHADER_chunk_vert[];
extern "C" const size_t SHADER_chunk_vert_len;

extern "C" const unsigned char SHADER_wireframe_frag[];
extern "C" const size_t SHADER_wireframe_frag_len;

extern "C" const unsigned char SHADER_wireframe_vert[];
extern "C" const size_t SHADER_wireframe_vert_len;

namespace client {
    void game::download_world(){
        boost::system::error_code err;
        for (int x = 0;x < WORLD_SIZE; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < WORLD_SIZE; z++) {
                    boost::array<long, 4096> arr{};
                    size_t len = boost::asio::read(socket, boost::asio::buffer(arr),err);
                    if (err == boost::asio::error::eof) {
                        printf("UNEXPECTED EOF WHILE READING CHUNKS %i %i %i\n", x, y, z);
                        std::exit(11);
                    } else if (err) {
                        throw boost::system::system_error(err);
                    }
                    world.map[x][z]->read(y, arr);
                }
            }
        }
    }

    void game::connect_to_server(const std::string&host,const std::string&port){
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::query query(host, port);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(query);

        boost::asio::connect(socket, endpoints);
    }

    game::game(GLFWwindow *window, const std::string &host, const std::string &port): socket(io_context) {
        this->window = window;

        try {
            printf("**************************************************\n\nBEGIN CONNECTION TO SERVER\n\n**************************************************\n\n");

            connect_to_server(host,port);
            download_world();
            read_welcome_packet();

        } catch (std::exception &e) {
            std::cerr << "client ex caught in initial connection: " << e.what()<< "\n";
            std::exit(11);
        }


        render_world();

        read_packet();

        io_context.run();

        initialize_gl();

    }

    void game::initialize_gl() {
        shader = new gl::shader(SHADER_chunk_vert, SHADER_chunk_vert_len, SHADER_chunk_frag,SHADER_chunk_frag_len);
        texture = new gl::texture(TEXTURE_1_8_textures_0_png, TEXTURE_1_8_textures_0_png_len);
        wireframe_shader=new gl::shader(SHADER_wireframe_vert,SHADER_wireframe_vert_len,SHADER_wireframe_frag,SHADER_wireframe_frag_len);
        text_rend=new text_renderer(window);
        gl::mesh_data data{
            {{3,{
                    0,0,0, 1,0,0,
                    0,0,0, 0,1,0,
                    0,0,0, 0,0,1,

                    1,0,0, 1,1,0,
                    1,0,0, 1,0,1,

                    0,1,0, 1,1,0,
                    0,1,0, 0,1,1,

                    0,0,1, 1,0,1,
                    0,0,1, 0,1,1,

                    0,1,1, 1,1,1,
                    1,0,1, 1,1,1,
                    1,1,0, 1,1,1
                }}},
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23}
        };
        for(int i=0;i<data.buffers[0].data.size();i++){
            data.buffers[0].data[i]-=0.5;
//            data.buffers[0].data[i+0]-=0.5;
//            data.buffers[0].data[i+1]-=0.5;
//            data.buffers[0].data[i+2]-=0.5;

//            data.buffers[0].data[i+0]*=2;
        }
        wireframe_mesh=new gl::mesh(&data);
        glfwSetWindowUserPointer(window,this);
        glfwSetKeyCallback(window,[](GLFWwindow*w,int key,int scancode,int actions,int mods){
            ((game*)glfwGetWindowUserPointer(w))->glfw_key_press_callback(key,scancode,actions,mods);
        });
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window,[](GLFWwindow*w,double x,double y){
            auto p=(game*)glfwGetWindowUserPointer(w);
            int a,b;
            glfwGetWindowSize(w,&a,&b);
            glm::vec2 diff{x-a/2,b/2-y};
            float pi=3.1415;
//            float pi=180;
            float dx=utils::map(diff.y,-10.0f,10.0f,pi/2,-pi/2);
            float dy=utils::map(diff.x,-10.0f,10.0f,pi,-pi);
            p->rotX=utils::clamp(0.01f*dx,-pi*0.49f,pi*0.49f);
            p->rotY=0.01f*dy;
            p->freecamLookdir={0,0,1};
            p->freecamLookdir=glm::rotateX(p->freecamLookdir,p->rotX);
            p->freecamLookdir=glm::rotateY(p->freecamLookdir,p->rotY);
//            std::cout<<x<<" "<<y<<" "<<dx<<" "<<dy<<" "<<p->rotX<<","<<p->rotY<<" "<<p->freecamLookdir.x<<" "<<p->freecamLookdir.y<<" "<<p->freecamLookdir.z<<"\n";
        });
    }

    void game::render_world(){
        for (int x = 0;x < WORLD_SIZE; x++) {
            for (int z = 0;z < WORLD_SIZE; z++) {
                rendered_world[x][z] =std::make_shared<rendered_chunk>(glm::ivec2{x, z});
                rendered_world[x][z]->take_chunk(world, world.map[x][z]);
                rendered_world[x][z]->render_chunk();
            }
        }
        std::cout<<"Rendered world\n";
    }

    void game::loop() {
        std::lock_guard<std::mutex>guard(protect_game_state);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.7, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p = glm::perspective(80.0F, 1.0F, 0.01F, 1000.0F);

        std::shared_ptr<nbt::nbt_list>nbt_cur_pos=nbt::cast_list(nbt::cast_compound(entities[player_id])->value["position"]);
        glm::vec3 curPos{nbt::cast_float(nbt_cur_pos->value[0])->value,nbt::cast_float(nbt_cur_pos->value[1])->value,nbt::cast_float(nbt_cur_pos->value[2])->value};

        glm::vec3 lookAt=curPos;
        glm::vec3 lookFrom=curPos-freecamLookdir;

        if(freecam){
            lookFrom=freecamPos;
            glm::vec3 forward=freecamLookdir;
            lookAt=freecamPos+forward;
            float dt=0.5;
            if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){
                freecamPos+=forward*dt;
            }
            if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){
                freecamPos-=forward*dt;
            }
            glm::vec3 left=glm::cross(forward,glm::vec3{0,-1,0});
            if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){
                freecamPos+=left*dt;
            }
            if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){
                freecamPos-=left*dt;
            }
            if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS)freecamPos.y+=dt;
            if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS)freecamPos.y-=dt;
        }

        text_rend->render_string("Hello World",300,500);

        glm::mat4 v=glm::lookAt(lookFrom,lookAt,{0,-1,0});

        shader->bind();
        shader->uniform4x4("perspective", p);
        shader->uniform4x4("view", v);
        shader->texture("tex", texture, 0);

        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int z = 0; z < WORLD_SIZE; z++) {
                rendered_world[x][z]->render(shader);
            }
        }

        wireframe_shader->bind();
        wireframe_shader->uniform4x4("perspective", p);
        wireframe_shader->uniform4x4("view", v);
        for(const auto& e:entities){
            std::shared_ptr<nbt::nbt_compound>ent=nbt::cast_compound(e.second);
            std::shared_ptr<nbt::nbt_list>pos=nbt::cast_list(ent->value["position"]);
            std::shared_ptr<nbt::nbt_list>look=nbt::cast_list(ent->value["lookdir"]);
            glm::mat4 m(1);
            m*=glm::translate(glm::mat4(1),glm::vec3{nbt::cast_float(pos->value[0])->value,nbt::cast_float(pos->value[1])->value,nbt::cast_float(pos->value[2])->value});
//            wireframe_shader->uniform4x4("model",m);
//            wireframe_shader->uniform3("color",glm::vec3{0,0,1});//BLUE=no rot, GREEN=with rot
//            wireframe_mesh->render_lines();
            m*=glm::lookAt(glm::vec3{0,0,0},glm::vec3{nbt::cast_float(look->value[0])->value,0,nbt::cast_float(look->value[2])->value},glm::vec3{0,-1,0});
            wireframe_shader->uniform4x4("model",m);
            wireframe_shader->uniform3("color",glm::vec3{0,1,0});
            wireframe_mesh->render_lines();
            //nbt::cast_float(look->value[1])->value
        }

        if(!freecam){
            std::shared_ptr<nbt::nbt>interaction_packet=nbt::make_compound({
                {"movement",nbt::make_compound({
                    {"left",nbt::make_short(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS)},
                    {"right",nbt::make_short(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)},
                    {"back",nbt::make_short(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS)},
                    {"front",nbt::make_short(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)},
                    {"sprint",nbt::make_short(glfwGetKey(window,GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS)},
                })},
                {"lookdir",nbt::make_list({nbt::make_float(freecamLookdir.x),nbt::make_float(freecamLookdir.y),nbt::make_float(freecamLookdir.z)})}
            });
            send_packet(interaction_packet);
        }

        if (glfwGetKey(window, GLFW_KEY_Q)) {
#ifdef I_HATE_CANCER
            while(sqrt(5)>0)std::raise(11);
#else
            while(sqrt(5)>0)std::exit(11);
#endif
        }

    }

    void game::end() {
        delete shader;
        delete texture;
        delete wireframe_shader;
        delete wireframe_mesh;
        delete text_rend;
    }

    void game::read_packet() {
        std::cout<<"packet read initiated\n";
        boost::thread t([this](){
            while(sqrt(5)>0){
                boost::array<long,1>arr{};
                boost::asio::read(socket,boost::asio::buffer(arr));
                unsigned long length_of_nbt=arr[0];
                boost::asio::streambuf read_buffer;
                boost::asio::read(socket,read_buffer,boost::asio::transfer_exactly(length_of_nbt));
                boost::asio::streambuf::const_buffers_type bufs=read_buffer.data();
                std::string str(boost::asio::buffers_begin(bufs),boost::asio::buffers_begin(bufs)+length_of_nbt);
                std::istringstream stream(str);
                std::shared_ptr<nbt::nbt>obj=nbt::read_nbt(stream);
                load_game_update(obj);
            }
        });
    }

    void game::send_packet(const std::shared_ptr<nbt::nbt>& data) {
        std::stringstream a;
        data->write(a);
        boost::array<unsigned long,1>size{a.str().length()};
        boost::asio::write(socket,boost::asio::buffer(size));
        boost::asio::write(socket,boost::asio::buffer(a.str()));
    }

    void game::load_game_update(const std::shared_ptr<nbt::nbt> &obj) {
        std::lock_guard<std::mutex>guard(protect_game_state);
        std::shared_ptr<nbt::nbt_compound> compound=nbt::cast_compound(obj);

        {
            std::shared_ptr<nbt::nbt_compound>nbt_ent=nbt::cast_compound(compound->value["entities"]);
            entities.clear();
            for(const auto& p:nbt_ent->value){
                entities[p.first]=p.second;
            }
        }

    }

    void game::read_welcome_packet() {
        boost::array<long,1>arr{};
        boost::asio::read(socket,boost::asio::buffer(arr));
        unsigned long length_of_nbt=arr[0];
        boost::asio::streambuf read_buffer;
        boost::asio::read(socket,read_buffer,boost::asio::transfer_exactly(length_of_nbt));
        boost::asio::streambuf::const_buffers_type bufs=read_buffer.data();
        std::string str(boost::asio::buffers_begin(bufs),boost::asio::buffers_begin(bufs)+length_of_nbt);
        std::istringstream stream(str);
        std::shared_ptr<nbt::nbt_compound>welcome_packet=nbt::cast_compound(nbt::read_nbt(stream));

        std::shared_ptr<nbt::nbt_compound>nbt_ent=nbt::cast_compound(welcome_packet->value["entities"]);
        entities.clear();
        for(const auto& p:nbt_ent->value){
            entities[p.first]=p.second;
        }

        player_id=nbt::cast_string(welcome_packet->value["player_id"])->value;
        std::cout<<"Welcome packet recieved:\n";
        std::cout<<"\tplayer_id = <"<<player_id<<">\n";
    }

    void game::glfw_key_press_callback(int key,int scancode,int actions,int mods) {
        if(key=='U'&&actions==GLFW_PRESS){
            freecam=!freecam;
            std::shared_ptr<nbt::nbt_list>nbt_cur_pos=nbt::cast_list(nbt::cast_compound(entities[player_id])->value["position"]);
            freecamPos={nbt::cast_float(nbt_cur_pos->value[0])->value,nbt::cast_float(nbt_cur_pos->value[1])->value,nbt::cast_float(nbt_cur_pos->value[2])->value};
        }
    }

}