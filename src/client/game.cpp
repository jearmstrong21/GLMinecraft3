//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "nbt/nbt.h"

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

    game::game(GLFWwindow *window, const std::string &host, const std::string &port): socket(io_context) {
        this->window = window;
        world = std::make_shared<block::world>();

        try {
            printf("START WORLD DOWNLOAD\n");

            boost::asio::ip::tcp::resolver resolver(io_context);
            boost::asio::ip::tcp::resolver::query query(host, port);
            boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(query);

            boost::asio::connect(socket, endpoints);

            boost::system::error_code err;

            printf("START READ\n");
            for (int x = 0;x < WORLD_SIZE; x++) {
                for (int y = 0; y < 16; y++) {
                    for (int z = 0; z < WORLD_SIZE; z++) {
                        boost::array<long, 4096> arr{};
                        size_t len = boost::asio::read(socket, boost::asio::buffer(arr),err);
                        if (err == boost::asio::error::eof) {
                            printf("UNEXPECTED EOF %i %i %i\n", x, y, z);
                            std::raise(11);
                        } else if (err) {
                            throw boost::system::system_error(err);
                        }
                        world->map[x][z]->read(y, arr);
                        printf("RECEIVED CHUNK %i %i %i\n", x, y, z);
                    }
                }
            }
            printf("END READ\n");

            read_packet();

            io_context.run();

//            boost::thread* m_thread;
//            m_thread = new boost::thread([&socket](){
//                boost::array<long, 1> arr{};
//                size_t len = boost::asio::read(socket, boost::asio::buffer(arr));
//
//                unsigned long length_of_nbt = arr[0];
//
//                std::cout << "Length: " << length_of_nbt << std::endl;
//
//                boost::asio::streambuf read_buffer;
//
//                len = boost::asio::read(socket, read_buffer,
//                                                      boost::asio::transfer_exactly(length_of_nbt));
//
//                boost::asio::streambuf::const_buffers_type bufs = read_buffer.data();
//                std::string str(boost::asio::buffers_begin(bufs),
//                                boost::asio::buffers_begin(bufs) + length_of_nbt);
//
//                std::istringstream stream(str);
//
//                std::shared_ptr<nbt::nbt> nbt_obj = nbt::read_nbt(stream);
//
//                std::cout << nbt_obj->to_str(" ") << std::endl;
//
////                delete[] despacito;
//            });
        } catch (
                std::exception &e
        ) {
            std::cerr << "client ex caught: " << e.

                    what()

                      << "\n";
            std::raise(11);
        }
        shader = std::make_shared<gl::shader>(SHADER_chunk_vert, SHADER_chunk_vert_len, SHADER_chunk_frag,
                                              SHADER_chunk_frag_len);
        texture = std::make_shared<gl::texture>(TEXTURE_1_8_textures_0_png, TEXTURE_1_8_textures_0_png_len);

        printf("START WORLD RENDER\n");
        for (
                int x = 0;
                x < WORLD_SIZE; x++) {
            for (
                    int z = 0;
                    z < WORLD_SIZE; z++) {
                rendered_world[x][z] =
                        std::make_shared<rendered_chunk>(glm::ivec2{x, z}
                        );
                rendered_world[x][z]->
                        take_chunk(world, world
                        ->map[x][z]);
                rendered_world[x][z]->

                        render_chunk();

            }
        }
        printf("END WORLD RENDER\n");
    }

    void game::loop() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.7, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p = glm::perspective(80.0F, 1.0F, 0.01F, 1000.0F);
        glm::mat4 v = glm::lookAt(glm::vec3(cos(glfwGetTime() * 0.25) * 16 * WORLD_SIZE / 2 + 16 * WORLD_SIZE / 2,
                                            sin(glfwGetTime() * 0.25) * 32 + 32,
                                            sin(glfwGetTime() * 0.25) * 16 * WORLD_SIZE / 2 + 16 * WORLD_SIZE / 2),
                                  glm::vec3(WORLD_SIZE * 8, 60, WORLD_SIZE * 8), glm::vec3(0, -1, 0));

        shader->bind();
        shader->uniform4x4("perspective", p);
        shader->uniform4x4("view", v);
        shader->texture("tex", texture, 0);

        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int z = 0; z < WORLD_SIZE; z++) {
                rendered_world[x][z]->render(shader);
            }
        }

//        if(glfwGetKey(window,GLFW_KEY_B)==GLFW_PRESS){
//            std::cout<<"SENDING BOOP\n";
            send_packet(nbt::make_compound({
                                                   {"source",nbt::make_string("game.cpp line 127")},
                                                   {"additional info",nbt::make_string("BOOP HAHA GOT UR NOSE")}
            }));
        std::cout<<"WRITEMSGS SIZE: "<<write_msgs.size()<<"\n";
//        }

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            std::raise(11);
        }
    }

    void game::end() {

    }

    void game::read_packet() {
        boost::thread t([this](){
            while(sqrt(5)>0){
                boost::array<long,1>arr{};
                boost::asio::read(socket,boost::asio::buffer(arr));
                std::cout<<"READ_PACKET\n";
                unsigned long length_of_nbt=arr[0];
                boost::asio::streambuf read_buffer;
                boost::asio::read(socket,read_buffer,boost::asio::transfer_exactly(length_of_nbt));
                boost::asio::streambuf::const_buffers_type bufs=read_buffer.data();
                std::string str(boost::asio::buffers_begin(bufs),boost::asio::buffers_begin(bufs)+length_of_nbt);
                std::istringstream stream(str);
                std::shared_ptr<nbt::nbt>obj=nbt::read_nbt(stream);
                std::cout<<obj->to_str("")<<"\n";
            }
        });
    }

    void game::send_packet(std::shared_ptr<nbt::nbt> data) {
//        boost::asio::post(io_context,[this,data](){
            bool wip=!write_msgs.empty();
            write_msgs.push_back(data);
            if(!wip){
                do_write();
            }
//        });
    }

    void game::do_write() {
        std::stringstream a;
        write_msgs[0]->write(a);
        boost::array<unsigned long,1>size{a.str().length()};
        boost::asio::write(socket,boost::asio::buffer(size));
        boost::asio::write(socket,boost::asio::buffer(a.str()));
        write_msgs.erase(write_msgs.begin());
        if(!write_msgs.empty())do_write();
    }

}