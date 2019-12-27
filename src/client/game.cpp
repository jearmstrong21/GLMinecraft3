//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
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

    game::game(GLFWwindow* window) {
        this->window = window;
    }

    void game::initialize() {
        world = std::make_shared<block::world>();
        shader = std::make_shared<gl::shader>(SHADER_chunk_vert, SHADER_chunk_vert_len, SHADER_chunk_frag,
                                              SHADER_chunk_frag_len);
        texture = std::make_shared<gl::texture>(TEXTURE_1_8_textures_0_png, TEXTURE_1_8_textures_0_png_len);
        basic_shader = std::make_shared<gl::shader>(SHADER_wireframe_vert, SHADER_wireframe_vert_len,
                                                    SHADER_wireframe_frag, SHADER_wireframe_frag_len);
        gl::mesh_data cube_data;
        cube_data.buffers.push_back({3, {
                // 000 001 010 011 100 101 110 111

                0, 0, 0, 1, 0, 0,
                0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 0, 1,

                0, 0, 1, 1, 0, 0,
                0, 0, 1, 0, 1, 0,
                0, 0, 1, 0, 0, 0,

                0, 1, 0, 1, 1, 0,
                0, 1, 0, 0, 0, 0,
                0, 1, 0, 0, 1, 1,

                0, 1, 1, 1, 1, 1,
                0, 1, 1, 0, 0, 1,
                0, 1, 1, 0, 1, 0,

                1, 0, 0, 0, 0, 0,
                1, 0, 0, 1, 1, 0,
                1, 0, 0, 1, 0, 1,

                1, 1, 0, 0, 1, 0,
                1, 1, 0, 1, 0, 0,
                1, 1, 0, 1, 1, 1,

                1, 1, 1, 0, 1, 1,
                1, 1, 1, 1, 0, 1,
                1, 1, 1, 1, 1, 0,

        }});
        cube_data.tri = {
                0, 1,
                2, 3,
                4, 5,
                6, 7,
                8, 9,
                10, 11,
                12, 13,
                14, 15,
                16, 17,
                18, 19,
                20, 21,
                22, 23,
                24, 25,
                26, 27,
                28, 29,
                30, 31,
                32, 33,
                34, 35,
                36, 37,
                38, 39,
                40, 41
        };
        basic_cube = std::make_shared<gl::mesh>(&cube_data);
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

        basic_shader->bind();
        basic_shader->uniform4x4("perspective", p);
        basic_shader->uniform4x4("view", v);
        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < WORLD_SIZE; z++) {
                    basic_shader->uniform4x4("mode", glm::translate(glm::mat4(1), {x * 16, y * 16, z * 16}));
                    basic_cube->render_lines();
                }
            }
        }

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            std::raise(11);
        }
    }

    void game::end() {

    }

    void game::download_world(std::string host, std::string port) {
        try {
            printf("START WORLD DOWNLOAD\n");
            boost::asio::io_context io_context;

            boost::asio::ip::tcp::resolver resolver(io_context);
            boost::asio::ip::tcp::resolver::query query(host, port);
            boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(query);

            boost::asio::ip::tcp::socket socket(io_context);
            boost::asio::connect(socket, endpoints);

            boost::system::error_code err;

            printf("START READ\n");
            for (int x = 0; x < WORLD_SIZE; x++) {
                for (int y = 0; y < 16; y++) {
                    for (int z = 0; z < WORLD_SIZE; z++) {
                        boost::array<long, 4096> arr{};
                        size_t len = boost::asio::read(socket, boost::asio::buffer(arr));
//                        printf("%zu %zu\n",len,sizeof(long),sizeof(long)*4096);
//                        boost::asio::read(socket,arr);
                        if (err == boost::asio::error::eof) {
                            printf("UNEXPECTED EOF %i %i %i\n", x, y, z);
                            std::raise(11);
                        } else if (err) {
                            throw boost::system::system_error(err);
                        }
                        world->map[x][z]->read(y, arr);
                        printf("RECIEVED CHUNK %i %i %i\n", x, y, z);
                    }
                }
            }
            printf("END READ\n");

//            boost::thread* m_thread;
//            m_thread = new boost::thread([&](){
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



            printf("START WORLD RENDER\n");
            for (int x = 0; x < WORLD_SIZE; x++) {
                for (int z = 0; z < WORLD_SIZE; z++) {
                    rendered_world[x][z] = std::make_shared<rendered_chunk>(glm::ivec2{x, z});
                    rendered_world[x][z]->take_chunk(world, world->map[x][z]);
                    rendered_world[x][z]->render_chunk();
                }
            }
            printf("END WORLD RENDER\n");
        } catch (std::exception& e) {
            std::cerr << "client ex caught: " << e.what() << "\n";
            std::raise(11);
        }
    }

}