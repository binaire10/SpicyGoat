//
// Created by thekinrar on 01/04/19.
//

#ifndef SPICYGOAT_SERVER_H
#define SPICYGOAT_SERVER_H

#include "entities/EntityPlayer.h"
#include "world/World.h"
#include <vector>
#include <uuid/uuid.h>
#include <string>

class Server {
public:
    inline static constexpr const char* const PROTOCOL_VERSION_NAME = "1.16.3";
    inline static const int PROTOCOL_VERSION_NUMBER = 753;

    inline static const int VIEW_DISTANCE = 10;
    inline static const int ENTITY_VIEW_DISTANCE = 100;
    inline static const int ENTITY_VIEW_DISTANCE_SQ = ENTITY_VIEW_DISTANCE * ENTITY_VIEW_DISTANCE;

    Server();
    static Server* get();

    EntityPlayer* createPlayer(uuid_t &uuid, std::string name, TCPConnection &conn);
    void removePlayer(EntityPlayer&);
    [[nodiscard]] const std::forward_list<Entity*>& getEntities() const;
    [[nodiscard]] const std::forward_list<EntityPlayer*>& getPlayers() const;
    int32_t nextEID();
    unsigned long getPlayerCount();

    void tick();

    World& getWorld();

    ChunkPalette *getPalette() const;

    void broadcastPacket(Packet*);

private:
    ChunkPalette *palette;

    std::forward_list<EntityPlayer*> players;
    int playerCount = 0;

    int32_t next_eid = 0;
    std::forward_list<Entity*> entities;

    World world;
};


#endif //SPICYGOAT_SERVER_H
