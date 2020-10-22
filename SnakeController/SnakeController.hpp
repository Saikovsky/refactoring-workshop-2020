#pragma once

#include <list>
#include <memory>

#include "IEventHandler.hpp"
#include "SnakeInterface.hpp"

class Event;
class IPort;

namespace Snake
{
struct ConfigurationError : std::logic_error
{
    ConfigurationError();
};

struct UnexpectedEventException : std::runtime_error
{
    UnexpectedEventException();
};

class Controller : public IEventHandler
{
public:

    Controller(IPort& p_displayPort, IPort& p_foodPort, IPort& p_scorePort, std::string const& p_config);

    Controller(Controller const& p_rhs) = delete;
    Controller& operator=(Controller const& p_rhs) = delete;

    void receive(std::unique_ptr<Event> e) override;

private:
    struct Segment
    {
        int x;
        int y;
        int ttl;
    };



    IPort& m_displayPort;
    IPort& m_foodPort;
    IPort& m_scorePort;

    std::pair<int, int> m_mapDimension;
    std::pair<int, int> m_foodPosition;

    Direction m_currentDirection;
    std::list<Segment> m_segments;

    Segment createNewSegment(const Segment& currentHead){
        Segment newHead;
        newHead.x = currentHead.x + ((m_currentDirection & 0b01) ? (m_currentDirection & 0b10) ? 1 : -1 : 0);
        newHead.y = currentHead.y + (not (m_currentDirection & 0b01) ? (m_currentDirection & 0b10) ? 1 : -1 : 0);
        newHead.ttl = currentHead.ttl;
        return newHead;
    }

    bool checkLostCondition(Segment &newHead)
    {
        for (auto segment : m_segments) {
            if (segment.x == newHead.x and segment.y == newHead.y)
            {
                return true;
            }
        }
        return false;
    }

};

} // namespace Snake
