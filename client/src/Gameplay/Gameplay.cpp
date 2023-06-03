/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** gameplay
*/

#include "src/Gameplay/Gameplay.hpp"

Gameplay::Gameplay(std::shared_ptr<Window> _window) : _window(_window), _map(Map(10, 10)), _currentCharacterId(0), _currentCharacterIndex(0)
{
    this->findPlayer();
}

void Gameplay::initPlayer(Vector3 pos)
{
    Character player(0, 0, pos);

    _characters.push_back(player);
}

void Gameplay::runPlayers(void)
{
    for (auto &character : this->_characters) {
        character.run();
    }
}

void Gameplay::findPlayer(void)
{
    std::vector<std::string> map = this->_map.getMap();
    float _x = 0.0f;
    float _y = 0.0f;

    for (std::size_t y = 0; y < this->_map.getheight(); y++) {
        for (std::size_t x = 0; x < this->_map.getwidth(); x++) {
            this->_map.setcubePosition({ _x, 0.0f, _y });
            if (map[y][x] == 'P')
                this->initPlayer({this->_map.getcubePosition().x, this->_map.getcubePosition().y + (float)1.1, this->_map.getcubePosition().z});
            _x += 2.0f;
        }
        _x = 0.0f;
        _y += 2.0f;
    }
}

void Gameplay::run(void)
{
    this->_map.run();
    this->drawMap();
    this->handleInput();
    this->runPlayers();
}

void Gameplay::setCurrentCharacter()
{
    if (this->_currentCharacterId >= this->_characters.size())
        this->_currentCharacterId = 0;
    for (std::size_t i = 0; i < this->_characters.size(); i++) {
        if (this->_currentCharacterId == i) {
            this->_currentCharacter = this->_characters[i];
            this->_currentCharacterId++;
            break;
        }
    }
}

void Gameplay::handleInput(void)
{
    if (this->_rayWindow.isKeyDown(KEY_ESCAPE))
        this->_window->setExit(true);
    if (this->_rayWindow.isKeyReleased(KEY_F1)) {
        this->setCurrentCharacter();
        this->_window->setCamera({_currentCharacter.getPosition().x, _currentCharacter.getPosition().y + (float)1.5, _currentCharacter.getPosition().z - (float)0.5}, { 10.0f, 2.0f, 10.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE);
    }
    if (this->_rayWindow.isKeyReleased(KEY_F2)) {
        this->setCurrentCharacter();
        this->_window->setCamera({_currentCharacter.getPosition().x, _currentCharacter.getPosition().y + (float)3.5, _currentCharacter.getPosition().z - (float)3.5}, { 0.6f, -4.5f, 10.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE);
    }
    if (this->_rayWindow.isKeyReleased(KEY_F3)) {
        this->_window->setCamera({ -5.0f, 15.0f, 10.0f }, { 10.0f, 2.0f, 10.0f }, { 0.0f, 1.0f, 0.0f }, 80.0f, CAMERA_PERSPECTIVE);
    }
}

void Gameplay::drawMap(void)
{
    float _x = 0.0f;
    float _y = 0.0f;

    for (std::size_t y = 0; y < this->_map.getheight(); y++) {
        for (std::size_t x = 0; x < this->_map.getwidth(); x++) {
            this->_map.setcubePosition({ _x, 0.0f, _y });
            this->_rayCube.drawCube(this->_map.getcubePosition(), 2.0f, 2.0f, 2.0f, GREEN);
            this->_rayCube.drawCubeWires(this->_map.getcubePosition(), 2.0f, 2.0f, 2.0f, WHITE);
        _x += 2.0f;
        }
        _x = 0.0f;
        _y += 2.0f;
    }
    this->_map.drawMineral(this->_map.getmodelBanana());
}
