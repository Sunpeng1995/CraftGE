#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__
#include <algorithm>
#include <glm/glm.hpp>
#include "Object.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

class Particle {
public:
    glm::vec3 position;
    float size;
    glm::vec4 color;
    glm::vec3 speed;
    float weight;
    float life;

    float camera_distance;

    Particle() : life(-1.0f), camera_distance(-1.0f) {}

    bool operator<(Particle &that) {
        return this->camera_distance > that.camera_distance;
    }
};

class ParticleSystem : public Object {
public:
    ParticleSystem(std::string name);
    ParticleSystem(std::string name, int max_particles);
    ParticleSystem(std::string name, glm::vec3 position);
    ParticleSystem(std::string name, int max_particles, glm::vec3 position);
    virtual void draw(Shader* shader);
    virtual void update(float delta_time);
    void setShader(Shader* shader);
    void setParticleTexture(Texture texture);
    void setCamera(Camera* camera);

    void setGenerateSpeed(int num_per_second);

    void setLifeTime(float life_time);
    void setLifeTimeVar(float life_time_var);

    void setSpeedNorm(float speed_norm);
    void setSpeedNormVar(float speed_norm_var);

    void setSpeedDir(glm::vec3 speed_dir);
    void setSpeedDirVar(glm::vec3 speed_dir_var);
    void setSpeedDirVar(float speed_dir_var);

    void setSpawnBoxWidth(float x);
    void setSpawnBoxHeight(float y);
    void setSpawnBoxLength(float z);
    void setSpawnBoxSize(glm::vec3 size);
    void setSpawnBoxSize(float size);
    void setSpawnBoxSize(float x, float y, float z);

    void setGravityNorm(float gravity_norm);

private:
    static const int MAX_PARTICLES = 100000;
    Particle mParticleContainer[MAX_PARTICLES];
    float mParticlesPositionBuffer[MAX_PARTICLES * 4];
    float mParticlesColorBuffer[MAX_PARTICLES * 4];
    const float mVertexBufferData[12] = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
        -0.5f,  0.5f,  0.0f,
         0.5f,  0.5f,  0.0f
    };
    GLuint VAO;
    GLuint quadVBO, positionsVBO, colorsVBO;
    Shader* mShader;
    Texture mTexture;
    Camera* mCamera;

    int mMaxParticles;
    int mLastUsedParticleIndex;
    int mParticleCount;

    int debug_count = 0;

    int mNewParticlesPerSecond;
    float mNewParticlesRemainder;
    float mLifeTime, mLifeTimeVar;
    float mSpeedNorm, mSpeedNormVar;
    glm::vec3 mSpeedDir, mSpeedDirVar;
    //    X               Y                Z
    float mSpawnBoxWidth, mSpawnBoxHeight, mSpawnBoxLength;
    float mGravityNorm;
    glm::vec3 mGravityDir;

    std::string mName;

    int findUnusedParticle();
    void respawnParticles(Particle &particle);
    void generateNewParticles(float delta_time);
    void fillBuffer(float delta_time);
    void updateBufferInGPU();
};

#endif // !__PARTICLE_SYSTEM_H__
