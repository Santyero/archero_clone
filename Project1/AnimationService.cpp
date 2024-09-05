#pragma once
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include "AnimationConfig.h"

namespace Game {
    class AnimationService {
    public:
        AnimationService(ITextureLoader& textureLoader, IRenderer& renderer)
            : textureLoader(textureLoader), renderer(renderer) {}

        int loadAnimationSet(const std::string& texturePath, const AnimationConfig& config) {
            int textureId = textureLoader.loadTexture(texturePath);
            if (textureId < 0) {
                throw std::runtime_error("Failed to load texture: " + texturePath);
            }

            AnimationSet animSet;
            animSet.textureId = textureId;
            animSet.idle = std::make_unique<Animation>(0.2f);
            animSet.idle->setFrames(config.idleFrames);
            animSet.running = std::make_unique<Animation>(0.125f);
            animSet.running->setFrames(config.runningFrames);
            animSet.shooting = std::make_unique<Animation>(0.1f);
            animSet.shooting->setFrames(config.shootingFrames);

            int animSetId = nextAnimSetId++;
            animationSets[animSetId] = std::move(animSet);
            return animSetId;
        }

        void updateAnimation(int animSetId, float deltaTime, const std::string& state) {
            auto it = animationSets.find(animSetId);
            if (it == animationSets.end()) {
                std::cerr << "Animation set with ID " << animSetId << " not found." << std::endl;
                return;
            }

            auto& animSet = it->second;
            if (state == "idle") animSet.idle->update(deltaTime);
            else if (state == "running") animSet.running->update(deltaTime);
            else if (state == "shooting") animSet.shooting->update(deltaTime);
            else {
                std::cerr << "Unknown animation state: " << state << std::endl;
            }
        }

        void renderAnimation(int animSetId, const std::string& state, SDL_Rect& destRect) {
            auto it = animationSets.find(animSetId);
            if (it == animationSets.end()) {
                std::cerr << "Animation set with ID " << animSetId << " not found." << std::endl;
                return;
            }

            auto& animSet = it->second;
            IAnimation* currentAnim = nullptr;

            if (state == "idle") currentAnim = animSet.idle.get();
            else if (state == "running") currentAnim = animSet.running.get();
            else if (state == "shooting") currentAnim = animSet.shooting.get();
            else {
                std::cerr << "Unknown animation state: " << state << std::endl;
                return;
            }

            if (currentAnim) {
                SDL_Rect sourceRect = currentAnim->getCurrentFrame();
                renderer.renderTexture(animSet.textureId, sourceRect, destRect);
            }
            else {
                std::cerr << "No animation found for state: " << state << std::endl;
            }
        }

    private:
        struct AnimationSet {
            int textureId;
            std::unique_ptr<IAnimation> idle;
            std::unique_ptr<IAnimation> running;
            std::unique_ptr<IAnimation> shooting;
        };

        ITextureLoader& textureLoader;
        IRenderer& renderer;
        std::unordered_map<int, AnimationSet> animationSets;
        int nextAnimSetId = 0;
    };
}