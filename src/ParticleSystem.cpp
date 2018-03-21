#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(std::string name) : 
    ParticleSystem(name, MAX_PARTICLES) {

}

ParticleSystem::ParticleSystem(std::string name, int max_particles) :
    ParticleSystem(name, max_particles, glm::vec3(0.0f)) {

}

ParticleSystem::ParticleSystem(std::string name, glm::vec3 position) :
    ParticleSystem(name, MAX_PARTICLES, position) {

}

ParticleSystem::ParticleSystem(std::string name, int max_particles, glm::vec3 position) :
    Object(position), mName(name), mLifeTime(2.0f), mLifeTimeVar(0.0f),
    mSpeedNorm(1.0f), mSpeedNormVar(0.0f),
    mSpeedDir(glm::vec3(0, 1.0f, 0)), mSpeedDirVar(glm::vec3(0)),
    mGravityDir(glm::vec3(0, -1.0f, 0)), mGravityNorm(9.8f),
    mTexture("res/textures/particle.png", "particle"){
    if (max_particles < MAX_PARTICLES) {
        mMaxParticles = max_particles;
    }
    else {
        mMaxParticles = MAX_PARTICLES;
    }
    mLastUsedParticleIndex = 0;
    mNewParticlesPerSecond = mMaxParticles / 10;

    mShader = new Shader("shader/particles/particles.vert", "shader/particles/particles.frag");

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexBufferData), mVertexBufferData, GL_STATIC_DRAW);

    glGenBuffers(1, &positionsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
    glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &colorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///////////////////////
    //////RANDOM///////////
    srand(42);
}

void ParticleSystem::draw(Shader* shader) {

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture.getTextureID());
    shader->setInt("particle_texture", 0);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mParticleCount);

    glBindVertexArray(0);
}

void ParticleSystem::update(float delta_time) {
    generateNewParticles(delta_time);
    fillBuffer(delta_time);
    updateBufferInGPU();
}

void ParticleSystem::setGenerateSpeed(int num_per_second) {
    if (num_per_second > (mMaxParticles / 10)) {
        mNewParticlesPerSecond = mMaxParticles / 10;
    }
    else {
        mNewParticlesPerSecond = num_per_second;
    }
}

int ParticleSystem::findUnusedParticle() {
    for (int i = mLastUsedParticleIndex; i < mMaxParticles; i++) {
        if (mParticleContainer[i].life < 0) {
            mLastUsedParticleIndex = i;
            return i;
        }
    }

    for (int i = 0; i < mLastUsedParticleIndex; i++) {
        if (mParticleContainer[i].life < 0) {
            mLastUsedParticleIndex = i;
            return i;
        }
    }

    mLastUsedParticleIndex = 0;
    std::cout << "full used" << std::endl;
    return 0;
}

void ParticleSystem::respawnParticles(Particle &particle) {
    glm::vec3 offset;
    offset.x = ((rand() % 100) - 50) / 100.0f;
    offset.y = ((rand() % 100) - 50) / 100.0f;
    offset.z = ((rand() % 100) - 50) / 100.0f;
    glm::vec3 color;
    color.x = ((rand() % 255)) / 255.0f;
    color.y = ((rand() % 255)) / 255.0f;
    color.z = ((rand() % 255)) / 255.0f;
    glm::vec3 speedvar;
    speedvar.x = ((rand() % 100) - 50) / 100.0f;
    speedvar.y = ((rand() % 100) - 50) / 100.0f;
    speedvar.z = ((rand() % 100) - 50) / 100.0f;

    particle.position = this->mPosition + offset * 0.4f;
    particle.size = 0.1f;
    particle.color = glm::vec4(color, 0.2f);
    particle.speed = glm::vec3(0.0f, 3.0f, 0.0f) + speedvar * 1.0f;
    particle.life = 2.5f;
}

void ParticleSystem::generateNewParticles(float delta_time) {
    int newparticles = (int)(delta_time * mNewParticlesPerSecond);
    if (newparticles > (int)(0.01667f * mNewParticlesPerSecond)) {
        newparticles = (int)(0.01667f * mNewParticlesPerSecond);
    }

    for (int i = 0; i < newparticles; i++) {
        int idx = findUnusedParticle();
        respawnParticles(mParticleContainer[idx]);
    }
}

void ParticleSystem::fillBuffer(float delta_time) {
    mParticleCount = 0;
    for (int i = 0; i < mMaxParticles; i++) {
        Particle &p = mParticleContainer[i];
        if (p.life > 0.0f) {
            p.life -= delta_time;
            
            if (p.life > 0.0f) {
                p.speed += glm::vec3(0.0f, -9.8f, 0.0f) * delta_time * 0.3f;
                p.position += p.speed * delta_time;
                //p.camera_distance = p.position.z;
                if (mCamera) p.camera_distance = glm::length(p.position - mCamera->getPos());

                mParticlesPositionBuffer[4 * mParticleCount + 0] = p.position.x;
                mParticlesPositionBuffer[4 * mParticleCount + 1] = p.position.y;
                mParticlesPositionBuffer[4 * mParticleCount + 2] = p.position.z;
                mParticlesPositionBuffer[4 * mParticleCount + 3] = p.size;

                mParticlesColorBuffer[4 * mParticleCount + 0] = p.color.r;
                mParticlesColorBuffer[4 * mParticleCount + 1] = p.color.g;
                mParticlesColorBuffer[4 * mParticleCount + 2] = p.color.b;
                mParticlesColorBuffer[4 * mParticleCount + 3] = p.color.a;

                mParticleCount++;
            }
            else {
                p.camera_distance = -1.0f;
            }
        }
    }
    std::sort(&mParticleContainer[0], &mParticleContainer[mMaxParticles]);
    if (debug_count == 60) {
        std::cout << "particle count: " << mParticleCount << std::endl;
        debug_count = 0;
    }
    else {
        debug_count++;
    }
}

void ParticleSystem::updateBufferInGPU() {
    glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
    glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mParticleCount * 4 * sizeof(float), mParticlesPositionBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mParticleCount * 4 * sizeof(float), mParticlesColorBuffer);
}

void ParticleSystem::setShader(Shader* shader) {
    auto old = mShader;
    mShader = shader;
    if (old) delete old;
}

void ParticleSystem::setParticleTexture(Texture texture) {
    mTexture = texture;
}

void ParticleSystem::setCamera(Camera* camera) {
    mCamera = camera;
}

void ParticleSystem::setLifeTime(float life_time) {
    mLifeTime = life_time;
}

void ParticleSystem::setLifeTimeVar(float life_time_var) {
    mLifeTimeVar = life_time_var;
}

void ParticleSystem::setSpeedNorm(float speed_norm) {
    mSpeedNorm = speed_norm;
}

void ParticleSystem::setSpeedNormVar(float speed_norm_var) {
    mSpeedNormVar = speed_norm_var;
}

void ParticleSystem::setSpeedDir(glm::vec3 speed_dir) {
    mSpeedDir = speed_dir;
}

void ParticleSystem::setSpeedDirVar(glm::vec3 speed_dir_var) {
    mSpeedDirVar = speed_dir_var;
}

void ParticleSystem::setSpeedDirVar(float speed_dir_var) {
    mSpeedDirVar = glm::vec3(speed_dir_var);
}
