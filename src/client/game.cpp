#include <climits>
//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "nbt/nbt.h"

#ifdef __APPLE__
#undef I_LOVE_CANCER
#else // Insane people
#def I_LOVE_CANCER
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

extern "C" const unsigned char TEXTURE_entity_steve[];
extern "C" const size_t TEXTURE_entity_steve_len;

namespace client {
    void game::download_world() {
        boost::system::error_code err;
        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < WORLD_SIZE; z++) {
                    boost::array<long, 4096> arr{};
                    size_t len = boost::asio::read(socket, boost::asio::buffer(arr), err);
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

    void game::connect_to_server(const std::string &host, const std::string &port) {
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::query query(host, port);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(query);

        boost::asio::connect(socket, endpoints);
    }

    game::game(GLFWwindow *window, const std::string &host, const std::string &port) : socket(io_context) {
        this->window = window;

        try {
            printf("**************************************************\n\nBEGIN CONNECTION TO SERVER\n\n**************************************************\n\n");

            connect_to_server(host, port);
            download_world();
            read_welcome_packet();

        } catch (std::exception &e) {
            std::cerr << "client ex caught in initial connection: " << e.what() << "\n";
            std::exit(11);
        }


        render_world();

        read_packet();

        io_context.run();

        initialize_gl();

    }

    void game::initialize_gl() {
        steve_texture = new gl::texture(TEXTURE_entity_steve,TEXTURE_entity_steve_len);
        shader = new gl::shader(SHADER_chunk_vert, SHADER_chunk_vert_len, SHADER_chunk_frag, SHADER_chunk_frag_len);
        texture = new gl::texture(TEXTURE_1_8_textures_0_png, TEXTURE_1_8_textures_0_png_len);
        wireframe_shader = new gl::shader(SHADER_wireframe_vert, SHADER_wireframe_vert_len, SHADER_wireframe_frag,
                                          SHADER_wireframe_frag_len);
        text_rend = new text_renderer(window);
        {
            gl::mesh_data data{
                    {{3, {
                                 0, 0, 0, 1, 0, 0,
                                 0, 0, 0, 0, 1, 0,
                                 0, 0, 0, 0, 0, 1,

                                 1, 0, 0, 1, 1, 0,
                                 1, 0, 0, 1, 0, 1,

                                 0, 1, 0, 1, 1, 0,
                                 0, 1, 0, 0, 1, 1,

                                 0, 0, 1, 1, 0, 1,
                                 0, 0, 1, 0, 1, 1,

                                 0, 1, 1, 1, 1, 1,
                                 1, 0, 1, 1, 1, 1,
                                 1, 1, 0, 1, 1, 1
                         }}},
                    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}
            };
            for (float &i : data.buffers[0].data) {
                i -= 0.5;
            }
            wireframe_mesh = new gl::mesh(&data);
        }
        {
            gl::mesh_data data{
                    {{3, {
                                 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0,
                                 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1,

                                 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1,
                                 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1,

                                 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1,
                                 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1,
                         }}},
                    {
                     0, 1, 2, 1, 2, 3,
                            4, 5, 6, 5, 6, 7,
                            8, 9, 10, 9, 10, 11,
                            12, 13, 14, 13, 14, 15,
                            16, 17, 18, 17, 18, 19,
                            20, 21, 22, 21, 22, 23
                    }
            };
            for (float &i : data.buffers[0].data) {
                i -= 0.5;
            }
            filledcube_mesh = new gl::mesh(&data);
        }
        {
            tc_renderer = new textured_cube_renderer();
        }
        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, [](GLFWwindow *w, int key, int scancode, int actions, int mods) {
            ((game *) glfwGetWindowUserPointer(w))->glfw_key_press_callback(key, scancode, actions, mods);
        });
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, [](GLFWwindow *w, double x, double y) {
            auto p = (game *) glfwGetWindowUserPointer(w);
            int a, b;
            glfwGetWindowSize(w, &a, &b);
            glm::vec2 diff{x - a / 2, b / 2 - y};
            float pi = 3.1415;
//            float pi=180;
            float dx = utils::map(diff.y, -10.0f, 10.0f, pi / 2, -pi / 2);
            float dy = utils::map(diff.x, -10.0f, 10.0f, pi, -pi);
            p->rotX = utils::clamp(0.01f * dx, -pi * 0.49f, pi * 0.49f);
            p->rotY = 0.01f * dy;
            p->lookdir = {0, 0, 1};
            p->lookdir = glm::rotateX(p->lookdir, p->rotX);
            p->lookdir = glm::rotateY(p->lookdir, p->rotY);
//            std::cout<<x<<" "<<y<<" "<<dx<<" "<<dy<<" "<<p->rotX<<","<<p->rotY<<" "<<p->lookdir.x<<" "<<p->lookdir.y<<" "<<p->lookdir.z<<"\n";
        });
        glfwSetCharCallback(window, [](GLFWwindow *w, unsigned int codepoint) {
            auto p = (game *) glfwGetWindowUserPointer(w);
            p->handle_char(codepoint);
        });
    }

    void game::handle_char(char c) {
        if (ignore_character) {
            ignore_character = false;
            return;
        }
        curchatbuffer += c;
    }

    void game::render_world() {
        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int z = 0; z < WORLD_SIZE; z++) {
                rendered_world[x][z] = std::make_shared<rendered_chunk>(glm::ivec2{x, z});
                rendered_world[x][z]->take_chunk(world, world.map[x][z]);
                rendered_world[x][z]->render_chunk();
            }
        }
        std::cout << "Rendered world\n";
    }

    void game::loop() {
        std::lock_guard<std::mutex> guard(protect_game_state);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.7, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p = glm::perspective(80.0F, 1.0F, 0.01F, 1000.0F);

        std::shared_ptr<nbt::nbt_list> nbt_cur_pos = nbt::cast_list(
                nbt::cast_compound(entities[player_id])->value["position"]);
        glm::vec3 curPos{nbt::cast_float(nbt_cur_pos->value[0])->value, nbt::cast_float(nbt_cur_pos->value[1])->value,
                         nbt::cast_float(nbt_cur_pos->value[2])->value};

        glm::vec3 lookAt = curPos;
        glm::vec3 lookFrom = curPos - lookdir;

        if (freecam) {
            lookFrom = freecamPos;
            glm::vec3 forward = lookdir;
            lookAt = freecamPos + forward;
            if (!is_chat_open) {
                float dt = 0.5;
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                    freecamPos += forward * dt;
                }
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                    freecamPos -= forward * dt;
                }
                glm::vec3 left = glm::cross(forward, glm::vec3{0, -1, 0});
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                    freecamPos += left * dt;
                }
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                    freecamPos -= left * dt;
                }
                if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)freecamPos.y += dt;
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)freecamPos.y -= dt;
            }
        }

        render_chat();

        glm::mat4 v = glm::lookAt(lookFrom, lookAt, {0, -1, 0});

        shader->bind();
        shader->uniform4x4("perspective", p);
        shader->uniform4x4("view", v);
        shader->texture("tex", texture, 0);

        for (auto &row : rendered_world) {
            for (const auto &chunk: row) {
                chunk->render(shader);
            }
        }

        wireframe_shader->bind();
        wireframe_shader->uniform4x4("perspective", p);
        wireframe_shader->uniform4x4("view", v);
        for (const auto &e:entities) {
            std::shared_ptr<nbt::nbt_compound> ent = nbt::cast_compound(e.second);
            std::shared_ptr<nbt::nbt_list> pos = nbt::cast_list(ent->value["position"]);
            std::shared_ptr<nbt::nbt_list> look = nbt::cast_list(ent->value["lookdir"]);
            std::shared_ptr<nbt::nbt_list> size = nbt::cast_list(ent->value["bbsize"]);
            glm::vec3 bbsize{nbt::cast_float(size->value[0])->value, nbt::cast_float(size->value[1])->value,
                             nbt::cast_float(size->value[2])->value};

            glm::mat4 m(1);
//            m*=glm::translate(glm::mat4(1),glm::vec3{nbt::cast_float(pos->value[0])->value,nbt::cast_float(pos->value[1])->value,nbt::cast_float(pos->value[2])->value});
//            m*=glm::lookAt(glm::vec3{0,0,0},glm::vec3{nbt::cast_float(look->value[0])->value,0,nbt::cast_float(look->value[2])->value},glm::vec3{0,-1,0});
//            wireframe_shader->uniform4x4("model",m);
//            wireframe_shader->uniform3("color",glm::vec3{0,1,0});
//            wireframe_mesh->render_lines();



            m = glm::mat4(1);
            m *= glm::translate(glm::mat4(1),
                                glm::vec3{nbt::cast_float(pos->value[0])->value, nbt::cast_float(pos->value[1])->value,
                                          nbt::cast_float(pos->value[2])->value});
            tc_renderer->render_cube(p, v, m, steve_texture, glm::vec3{8,12,4},glm::vec2{16,32},glm::vec2{64,64});
//            m *= glm::scale(glm::mat4(1), bbsize);
//            wireframe_shader->uniform4x4("model", m);
//            wireframe_shader->uniform3("color", glm::vec3{1, 0, 0});
//            wireframe_mesh->render_lines();
//
//            m = glm::mat4(1);
//            m *= glm::translate(glm::mat4(1),
//                                glm::vec3{nbt::cast_float(pos->value[0])->value, nbt::cast_float(pos->value[1])->value,
//                                          nbt::cast_float(pos->value[2])->value});
//            m *= glm::scale(glm::mat4(1), glm::vec3{0.1});
//            wireframe_shader->uniform4x4("model", m);
//            wireframe_shader->uniform3("color", glm::vec3{0, 0, 1});
//            filledcube_mesh->render_triangles();
        }

        if (!freecam) {
            std::shared_ptr<nbt::nbt> interaction_packet = nbt::make_compound({
                                                                                      {"movement", nbt::make_compound({
                                                                                                                              {"left",   nbt::make_short(
                                                                                                                                      !is_chat_open &&
                                                                                                                                      glfwGetKey(
                                                                                                                                              window,
                                                                                                                                              GLFW_KEY_A) ==
                                                                                                                                      GLFW_PRESS)},
                                                                                                                              {"right",  nbt::make_short(
                                                                                                                                      !is_chat_open &&
                                                                                                                                      glfwGetKey(
                                                                                                                                              window,
                                                                                                                                              GLFW_KEY_D) ==
                                                                                                                                      GLFW_PRESS)},
                                                                                                                              {"back",   nbt::make_short(
                                                                                                                                      !is_chat_open &&
                                                                                                                                      glfwGetKey(
                                                                                                                                              window,
                                                                                                                                              GLFW_KEY_S) ==
                                                                                                                                      GLFW_PRESS)},
                                                                                                                              {"front",  nbt::make_short(
                                                                                                                                      !is_chat_open &&
                                                                                                                                      glfwGetKey(
                                                                                                                                              window,
                                                                                                                                              GLFW_KEY_W) ==
                                                                                                                                      GLFW_PRESS)},
                                                                                                                              {"sprint", nbt::make_short(
                                                                                                                                      !is_chat_open &&
                                                                                                                                      glfwGetKey(
                                                                                                                                              window,
                                                                                                                                              GLFW_KEY_LEFT_CONTROL) ==
                                                                                                                                      GLFW_PRESS)},
                                                                                                                              {"jump",   nbt::make_short(
                                                                                                                                      !is_chat_open &&
                                                                                                                                      should_jump
                                                                                                                              )}
                                                                                                                      })},
                                                                                      {"lookdir",  nbt::make_list(
                                                                                              {nbt::make_float(
                                                                                                      lookdir.x),
                                                                                               nbt::make_float(
                                                                                                       lookdir.y),
                                                                                               nbt::make_float(
                                                                                                       lookdir.z)})},
                                                                                      {"chat",     nbt::make_string(
                                                                                              chattosend)}
                                                                              });
            should_jump = false;
            chattosend = "";
            send_packet(interaction_packet);
        }

        if (glfwGetKey(window, GLFW_KEY_Q) && !is_chat_open) {
#ifdef I_LOVE_CANCER
            while(sqrt(5)>0)std::exit(11);
#else
            while (sqrt(5) > 0)std::raise(11);
#endif
        }

    }

    void game::end() {
        delete shader;
        delete texture;
        delete wireframe_shader;
        delete wireframe_mesh;
        delete filledcube_mesh;
        delete tc_renderer;
        delete text_rend;
        delete steve_texture;
    }

    void game::read_packet() {
        std::cout << "packet read initiated\n";
        boost::thread t([this]() {
            while (sqrt(5) > 0) {
                boost::array<long, 1> arr{};
                boost::asio::read(socket, boost::asio::buffer(arr));
                unsigned long length_of_nbt = arr[0];
                boost::asio::streambuf read_buffer;
                boost::asio::read(socket, read_buffer, boost::asio::transfer_exactly(length_of_nbt));
                boost::asio::streambuf::const_buffers_type bufs = read_buffer.data();
                std::string str(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + length_of_nbt);
                std::istringstream stream(str);
                std::shared_ptr<nbt::nbt> obj = nbt::read_nbt(stream);
                load_game_update(obj);
//                std::cout<<obj->to_str("")<<"\n";
            }
        });
    }

    void game::send_packet(const std::shared_ptr<nbt::nbt> &data) {
        std::stringstream a;
        data->write(a);
        boost::array<unsigned long, 1> size{a.str().length()};
        boost::asio::write(socket, boost::asio::buffer(size));
        boost::asio::write(socket, boost::asio::buffer(a.str()));
    }

    void game::load_game_update(const std::shared_ptr<nbt::nbt> &obj) {
        std::lock_guard<std::mutex> guard(protect_game_state);
        std::shared_ptr<nbt::nbt_compound> compound = nbt::cast_compound(obj);

        {
            std::shared_ptr<nbt::nbt_compound> nbt_ent = nbt::cast_compound(compound->value["entities"]);
            entities.clear();
            for (const auto &p:nbt_ent->value) {
                entities[p.first] = p.second;
            }
        }
        {
            std::string nchat = nbt::cast_string(compound->value["chat"])->value;
            if (!nchat.empty()) {
                chathistory.insert(chathistory.begin(), nchat);
            }
        }

    }

    void game::read_welcome_packet() {
        boost::array<long, 1> arr{};
        boost::asio::read(socket, boost::asio::buffer(arr));
        unsigned long length_of_nbt = arr[0];
        boost::asio::streambuf read_buffer;
        boost::asio::read(socket, read_buffer, boost::asio::transfer_exactly(length_of_nbt));
        boost::asio::streambuf::const_buffers_type bufs = read_buffer.data();
        std::string str(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + length_of_nbt);
        std::istringstream stream(str);
        std::shared_ptr<nbt::nbt_compound> welcome_packet = nbt::cast_compound(nbt::read_nbt(stream));

        std::shared_ptr<nbt::nbt_compound> nbt_ent = nbt::cast_compound(welcome_packet->value["entities"]);
        entities.clear();
        for (const auto &p:nbt_ent->value) {
            entities[p.first] = p.second;
        }

        player_id = nbt::cast_string(welcome_packet->value["player_id"])->value;
        std::cout << "Welcome packet recieved:\n";
        std::cout << "\tplayer_id = <" << player_id << ">\n";
    }

    void game::glfw_key_press_callback(int key, int __unused scancode, int actions, int __unused mods) {
        if (key == 'U' && actions == GLFW_PRESS) {
            freecam = !freecam;
            std::shared_ptr<nbt::nbt_list> nbt_cur_pos = nbt::cast_list(
                    nbt::cast_compound(entities[player_id])->value["position"]);
            freecamPos = {nbt::cast_float(nbt_cur_pos->value[0])->value, nbt::cast_float(nbt_cur_pos->value[1])->value,
                          nbt::cast_float(nbt_cur_pos->value[2])->value};
        }
        if (key == 'T' && actions == GLFW_PRESS && !is_chat_open) {
            is_chat_open = true;
            curchatbuffer = "";
            ignore_character = true;
        }
        if (key == GLFW_KEY_ESCAPE) {
            curchatbuffer = "";
            is_chat_open = false;
        }
        if (key == GLFW_KEY_SPACE && !is_chat_open)should_jump = true;
        if (key == GLFW_KEY_BACKSPACE && actions == GLFW_PRESS && is_chat_open && !curchatbuffer.empty()) {
            curchatbuffer.pop_back();
        }
        if (key == GLFW_KEY_ENTER && actions == GLFW_PRESS && is_chat_open) {
            std::cout << curchatbuffer << "\n";
            chattosend = curchatbuffer;
            curchatbuffer = "";
        }
    }

    void game::render_chat() {
//        glDisable(GL_DEPTH_TEST);
        if (is_chat_open) {
            text_rend->render_string(curchatbuffer, 0, text_rend->charsize);
            text_rend->render_string("                    ", 0, text_rend->charsize);
        }
        for (int i = 0; i < utils::min((int) chathistory.size(), 20); i++) {
//            text_rend->render_string("                    ",0,text_rend->charsize);
            text_rend->render_string(chathistory[i], 0, text_rend->charsize * (i + 2));
        }
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        text_rend->render_string("GLMinecraft3\nYOUR PLAYER ID: " + player_id + "\n", 0, h - text_rend->charsize);
//        glEnable(GL_DEPTH_TEST);
    }

}