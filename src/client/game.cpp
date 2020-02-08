#include <climits>
//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <item/registry.h>
#include "nbt/nbt.h"

extern "C" const unsigned char TEXTURE_1_8_textures_0_png[];
extern "C" const size_t TEXTURE_1_8_textures_0_png_len;

extern "C" const unsigned char SHADER_chunk_frag[];
extern "C" const size_t SHADER_chunk_frag_len;

extern "C" const unsigned char SHADER_chunk_vert[];
extern "C" const size_t SHADER_chunk_vert_len;

extern "C" const unsigned char TEXTURE_gui_widgets_png[];
extern "C" const size_t TEXTURE_gui_widgets_png_len;

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
        gui_widgets_texture = new gl::texture(TEXTURE_gui_widgets_png, TEXTURE_gui_widgets_png_len);
        shader = new gl::shader(SHADER_chunk_vert, SHADER_chunk_vert_len, SHADER_chunk_frag, SHADER_chunk_frag_len);
        texture = new gl::texture(TEXTURE_1_8_textures_0_png, TEXTURE_1_8_textures_0_png_len);
        text_rend = new text_renderer(window);
        item_rend = new item_renderer();
        box_rend = new box_renderer();
        ent_rend = new entity_render();
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
                rendered_world[x][z]->take_chunk(&world, world.map[x][z]);
                rendered_world[x][z]->render_chunk();
            }
        }
        std::cout << "Rendered world\n";
    }

    void game::loop() {
        {
            std::lock_guard<std::mutex> guard(protect_game_state);
            glfwGetWindowSize(window, &width, &height);
            glfwGetFramebufferSize(window, &fboWidth, &fboHeight);

            player = std::dynamic_pointer_cast<entity::entity_player>(
                    entities[player_id]);

            glViewport(0, 0, fboWidth, fboHeight);
            glClearColor(0.5, 0.7, 0.9, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            glm::mat4 p = glm::perspective(80.0F, 1.0F, 0.01F, 1000.0F);

            glm::vec3 curPos = player->box.pos;
            glm::vec3 curSize = player->box.size;

            glm::vec3 lookAt = curPos + glm::vec3{0, curSize.y, 0};
            glm::vec3 lookFrom = curPos - lookdir + glm::vec3{0, curSize.y, 0};

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

            ctx3d ctx_3d{p, v};

            for (const auto &e:entities) {
                int id = e.second->type_id;
                if (id == ENTITY_ID_PLAYER)ent_rend->render_player(p, v, e.second, texture, item_rend);
                if (id == ENTITY_ID_ZOMBIE)ent_rend->render_zombie(p, v, e.second);

                glm::vec3 pos = e.second->box.pos;
                glm::vec3 size = e.second->box.size;

                box_rend->render_lines(ctx_3d, e.second->box);
                box_rend->render_lines(ctx_3d, {e.second->box.pos, glm::vec3{1, 2, 1}});
            }

            if (!freecam) {
                int new_selected_pos = -1;
                if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)new_selected_pos = 0;
                if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)new_selected_pos = 1;
                if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)new_selected_pos = 2;
                if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)new_selected_pos = 3;
                if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)new_selected_pos = 4;
                if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)new_selected_pos = 5;
                if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)new_selected_pos = 6;
                if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)new_selected_pos = 7;
                if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)new_selected_pos = 8;
                std::shared_ptr<nbt::nbt> interaction_packet = nbt::nbt_compound::make({
                                                                                               {"leftclick",nbt::nbt_short::make(false)},
                                                                                               {"rightclick",nbt::nbt_short::make(false)},
                                                                                               {"new_selected_pos", nbt::nbt_int::make(
                                                                                                       new_selected_pos)},
                                                                                               {"movement",         nbt::nbt_compound::make(
                                                                                                       {
                                                                                                               {"left",   nbt::nbt_short::make(
                                                                                                                       !is_chat_open &&
                                                                                                                       glfwGetKey(
                                                                                                                               window,
                                                                                                                               GLFW_KEY_A) ==
                                                                                                                       GLFW_PRESS)},
                                                                                                               {"right",  nbt::nbt_short::make(
                                                                                                                       !is_chat_open &&
                                                                                                                       glfwGetKey(
                                                                                                                               window,
                                                                                                                               GLFW_KEY_D) ==
                                                                                                                       GLFW_PRESS)},
                                                                                                               {"back",   nbt::nbt_short::make(
                                                                                                                       !is_chat_open &&
                                                                                                                       glfwGetKey(
                                                                                                                               window,
                                                                                                                               GLFW_KEY_S) ==
                                                                                                                       GLFW_PRESS)},
                                                                                                               {"front",  nbt::nbt_short::make(
                                                                                                                       !is_chat_open &&
                                                                                                                       glfwGetKey(
                                                                                                                               window,
                                                                                                                               GLFW_KEY_W) ==
                                                                                                                       GLFW_PRESS)},
                                                                                                               {"sprint", nbt::nbt_short::make(
                                                                                                                       !is_chat_open &&
                                                                                                                       glfwGetKey(
                                                                                                                               window,
                                                                                                                               GLFW_KEY_LEFT_CONTROL) ==
                                                                                                                       GLFW_PRESS)},
                                                                                                               {"jump",   nbt::nbt_short::make(
                                                                                                                       !is_chat_open &&
                                                                                                                       glfwGetKey(
                                                                                                                               window,
                                                                                                                               GLFW_KEY_SPACE) ==
                                                                                                                       GLFW_PRESS
                                                                                                               )}
                                                                                                       })},
                                                                                               {"lookdir",          utils::cast3(
                                                                                                       lookdir)},
                                                                                               {"chat",             nbt::nbt_string::make(
                                                                                                       chattosend)}
                                                                                       });
                chattosend = "";
                send_packet(interaction_packet);
            }
        }

        if (glfwGetKey(window, GLFW_KEY_Q) && !is_chat_open) {
            attemptQuit = true;
            glfwSetWindowShouldClose(window, true);
        }

        while(!dirty_chunk_sections.empty()){
            glm::ivec3 v=dirty_chunk_sections.top();
            dirty_chunk_sections.pop();
            if(v.x<0||v.y<0||v.z<0||v.x>=WORLD_SIZE||v.y>=16||v.z>=WORLD_SIZE)continue;
            rendered_world[v.x][v.z]->take_chunk(&world,world.map[v.x][v.z],v.y);
            rendered_world[v.x][v.z]->render_chunk(v.y);
        }

        render_hud();

    }

    void game::end() {
        delete shader;
        delete texture;
        delete box_rend;
        delete ent_rend;
        delete text_rend;
        delete item_rend;
        delete gui_widgets_texture;
    }

    void game::read_packet() {
        std::cout << "packet read initiated\n";
        boost::thread t([this]() {
            if (attemptQuit)return;
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
//        std::cout<<"load_game_update "<<obj->to_str("")<<"\n";

        {
            entities.clear();
            for (const auto &p:obj->compound_ref()["entities"]->compound_ref()) {
                entities[p.first] = entity::load(nbt::cast_compound(p.second));
            }
        }
        {
            std::string nchat = obj->compound_ref()["chat"]->as_string();
            if (!nchat.empty()) {
                chathistory.insert(chathistory.begin(), nchat);
            }
        }
        {
            std::vector<nbt::nbt_ptr> list=obj->compound_ref()["world_ops"]->list_ref();
            for(auto&p:list){
                block::world_op o;
                o.load(nbt::cast_compound(p));
                int cx=o.pos.x/16;
                int cy=o.pos.y/16;
                int cz=o.pos.z/16;
                world.apply(o);
                dirty_chunk_sections.push({cx,cy,cz});
                if(o.pos.x%16==0)dirty_chunk_sections.push({cx-1,cy,cz});
                if(o.pos.x%16==15)dirty_chunk_sections.push({cx+1,cy,cz});
                if(o.pos.y%16==0)dirty_chunk_sections.push({cx,cy-1,cz});
                if(o.pos.y%16==15)dirty_chunk_sections.push({cx,cy+1,cz});
                if(o.pos.z%16==0)dirty_chunk_sections.push({cx,cy,cz-1});
                if(o.pos.z%16==15)dirty_chunk_sections.push({cx,cy,cz+1});
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
        std::shared_ptr<nbt::nbt> welcome_packet = nbt::read_nbt(stream);
        entities.clear();
        for (const auto &p:welcome_packet->compound_ref()["entities"]->compound_ref()) {
            entities[p.first] = entity::load(nbt::cast_compound(p.second));
        }

        player_id = welcome_packet->compound_ref()["player_id"]->as_string();
        std::cout << "Welcome packet recieved:\n";
        std::cout << "\tplayer_id = <" << player_id << ">\n";
    }

    void game::open_chat() {
        is_chat_open = true;
        curchatbuffer = "";
        ignore_character = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void game::close_chat() {
        curchatbuffer = "";
        is_chat_open = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void game::glfw_key_press_callback(int key, int scancode, int actions, int mods) {
        if (key == 'U' && actions == GLFW_PRESS) {
            freecam = !freecam;
            freecamPos = entities[player_id]->box.pos;
        }
        if (key == 'T' && actions == GLFW_PRESS && !is_chat_open) {
            open_chat();
        }
        if (key == GLFW_KEY_ESCAPE) {
            close_chat();
        }
        if (key == GLFW_KEY_BACKSPACE && actions == GLFW_PRESS && is_chat_open && !curchatbuffer.empty()) {
            curchatbuffer.pop_back();
        }
        if (key == GLFW_KEY_ENTER && actions == GLFW_PRESS && is_chat_open) {
            std::cout << curchatbuffer << "\n";
            chattosend = curchatbuffer;
            close_chat();
        }
    }

    void game::render_chat() {
        if (is_chat_open) {
            text_rend->render_string(curchatbuffer, 0, text_rend->charsize);
            text_rend->render_string("                    ", 0, text_rend->charsize);
        }
        for (int i = 0; i < utils::min((int) chathistory.size(), 20); i++) {
            text_rend->render_string(chathistory[i], 0, text_rend->charsize * (i + 2));
        }
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        std::string s = "GLMinecraft3\nYOUR PLAYER ID: " + player_id + "\nFACEDIR " +
                        std::to_string(player->facedir.x) + "," +
                        std::to_string(player->facedir.y) + "," +
                        std::to_string(player->facedir.z) + "\n";
        text_rend->render_string(s, 0, h - text_rend->charsize);
    }

    void game::render_hud() {
        glDisable(GL_DEPTH_TEST);
        render_chat();

        gui_ctx ctx{};
        ctx.width = fboWidth;
        ctx.height = fboHeight;
        ctx.ortho = glm::translate(glm::mat4(1), glm::vec3{-1, -1, 0}) *
                    glm::scale(glm::mat4(1), glm::vec3{4}) *
                    glm::scale(glm::mat4(1), glm::vec3{1.0 / fboWidth, 1.0 / fboHeight, 0});
        int scale = 3;
        int hotbar_y = 100;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        int center = width / 2;
        item_rend->render_texture(ctx, gui_widgets_texture, {0.0 / 256.0, 234.0 / 256.0}, {182.0 / 256.0, 22.0 / 256.0},
                                  {1, 1, 1},

                                  center - scale * 182 / 2,
                                  hotbar_y,
                                  scale * 182,
                                  scale * 22

        );
        item_rend->render_texture(ctx, gui_widgets_texture, {0.0 / 256.0, 210.0 / 256.0}, {24.0 / 256.0, 24.0 / 256.0},
                                  {1, 1, 1},

                                  center - scale * 182 / 2 + scale * 20 * player->selected_item,
                                  hotbar_y - scale,
                                  scale * 24,
                                  scale * 24

        );
        glDisable(GL_BLEND);
        for (int i = 0; i < 9; i++) {
            if (player->inventory[i].is_empty())continue;
            item_rend->render_item(
                    ctx, player->inventory[i].item()->render(player->inventory[i]), texture,
                    center - scale * 182 / 2 + scale * 20 * i + scale * 3,
                    hotbar_y - scale + scale * 4,
                    scale * 16
            );
        }
    }

}