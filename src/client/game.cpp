//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"
#include <thread>

namespace client {

    game::game(GLFWwindow *window) {
        this->window = window;
    }

    void game::initialize() {

        std::clock_t start,end;

        start=std::clock();
        std::shared_ptr<block::world> world(new block::world());
        end=std::clock();
        printf("WORLDINIT took %f\n",(end-start)/double(CLOCKS_PER_SEC));

        start=std::clock();
        std::vector<glm::ivec2>chunksToMesh;
        for(int x=0;x<WORLD_SIZE;x++){
            for(int z=0;z<WORLD_SIZE;z++){
                chunksToMesh.emplace_back(x,z);
            }
        }
        //1.30S 1T
        //1.91S 2T
        //2.49S 256T
        int numThreads=1;
        int threadSpan=chunksToMesh.size()/numThreads;
        std::vector<std::thread>threads;
        threads.reserve(numThreads);
        for(int i=0;i<numThreads;i++){
            threads.emplace_back([&](int threadIndex){
                for(int t=threadIndex*threadSpan;t<(threadIndex+1)*threadSpan;t++){
                    glm::ivec2 v=chunksToMesh[t];
                    rendered_world[v.x][v.y]=std::make_shared<rendered_chunk>(v);
                    rendered_world[v.x][v.y]->take_chunk(world,world->map[v.x][v.y]);
                }
            },i);
        }
        for(auto&t:threads){
            t.join();
        }
        end=std::clock();
        printf("RENDER took %f\n",(end-start)/double(CLOCKS_PER_SEC));

        start=std::clock();
        shader = std::shared_ptr<gl::shader>(new gl::shader("test", "test"));
        texture = std::shared_ptr<gl::texture>(new gl::texture("1.8_textures_0.png"));
        for(int x=0;x<WORLD_SIZE;x++){
            for(int z=0;z<WORLD_SIZE;z++){
                rendered_world[x][z]->render_chunk();
            }
        }
        end=std::clock();
        printf("GLINIT took %f\n",(end-start)/double(CLOCKS_PER_SEC));
    }

    void game::loop() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.7, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p = glm::perspective(80.0F, 1.0F, 0.01F, 1000.0F);
        glm::mat4 v = glm::lookAt(glm::vec3(cos(glfwGetTime()*0.25)*16*WORLD_SIZE/2+16*WORLD_SIZE/2,sin(glfwGetTime()*0.25)*32+64,sin(glfwGetTime()*0.25)*16*WORLD_SIZE/2+16*WORLD_SIZE/2), glm::vec3(WORLD_SIZE*8, 60, WORLD_SIZE*8), glm::vec3(0, -1, 0));

        shader->bind();
        shader->uniform4x4("perspective", p);
        shader->uniform4x4("view", v);
        shader->texture("tex", texture, 0);
        for(int x=0;x<WORLD_SIZE;x++){
            for(int z=0;z<WORLD_SIZE;z++){
                rendered_world[x][z]->render(shader);
            }
        }

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            std::raise(11);
        }
    }

    void game::end() {

    }

}