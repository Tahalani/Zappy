/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** character
*/

#ifndef CHARACTER_HPP_
    #define CHARACTER_HPP_

    #include <vector>
    #include <map>
    #include <chrono>
    #include <memory>
    #include <exception>
    #include <cstdlib>
    #include "encapsulation/Raylibcpp.hpp"
    #include "Inventory.hpp"

enum Directions {
    EAST = 90,
    SOUTH = 0,
    WEST = -90,
    NORTH = 180
};

enum Animations {
    SPAWN,
    DYING,
    RIGHT_TURN,
    LEFT_TURN,
    TAKING,
    NONE
};

class Character {
    public:
        class Error : public std::exception {
            public:
                Error(std::string const &message) : _message(message) {};
                ~Error() = default;
                const char *what() const noexcept override { return _message.c_str(); }
            private:
                std::string _message;
        };
        Character() = default;
        Character(std::size_t animsCount, std::size_t animFrameCounter, Vector3 pos, std::size_t level, std::size_t orientation, std::string name, std::size_t id, std::map<std::size_t, Texture2D> textures, std::vector<ModelAnimation *> _animations);
        ~Character();
        Vector3 getPosition() const;
        Model getModel() const;
        void chooseAnimation(Animations anim);
        int getAnimFrameCounter() const;
        ModelAnimation *getAnimation(Animations anim) const;
        void setAnimFrameCounter(int animFrameCounter);
        Animations getCurrentlyAnimation() const;
        void setCurrentlyAnimation(Animations currentlyAnimation);
        void setPos(float x, float z, int orientation);
        void handleEvent();
        void run();
        void checkLevel();
        void draw();
        size_t getLevel() const;
        std::string getTeamName() const;
        void setDirection(Directions direction);
        void setLevel(size_t level);
        Directions getDirection() const;
        std::size_t getId() const;
        std::map<std::size_t, Texture2D> getTextures() const;
        std::shared_ptr<Inventory> &getInventory();
        void setBroadMessage(std::string message);
        std::string getBroadMessage() const;
    private:
        Model _model;
        std::vector<ModelAnimation *> _animations;
        std::map<std::size_t, Texture2D> _textures;
        Vector3 _position;
        Vector3 _pos_temp;
        unsigned int _animsCount;
        int _animFrameCounter;
        Raylibcpp::RayModel _rayModel;
        Animations _currentlyAnimation;
        Directions _currentDirection;
        std::shared_ptr<Inventory> _inventory;
        std::size_t _level;
        std::size_t _levelTmp;
        std::size_t _id;
        std::string _teamname;
        std::string _broadmessage;
        std::chrono::steady_clock::time_point _startTime;
        std::chrono::steady_clock::time_point _currentTime;
        std::chrono::duration<double> _elapsedSeconds;
};

#endif /* !CHARACTER_HPP_ */
