#include "TriggerEvent.h"
#include "RewardFactory.h"
#include "RewardCommandManager.h"
#include <iostream>

int main() {
    // 1. Inisialisasi Manager
    RewardCommandManager manager;

    std::cout << "=== SIMULASI SKIP BLIND REWARD SYSTEM (BALATRO) ===\n\n";

    // 2. Simulasi Kejadian: Player melakukan Skip Blind dan mendapat Tag [cite: 54]
    std::cout << "[Aksi] Player melakukan SKIP BLIND. Mendapat Tag: Free Joker & Mega Blind Buff.\n";
    
    // Factory membuat objek reward secara tertunda (Deferred) [cite: 17, 56]
    auto reward1 = RewardFactory::createReward(TagType::FreeJoker);
    auto reward2 = RewardFactory::createReward(TagType::MegaBlindBuff);

    // Manager menyimpan objek reward tersebut [cite: 41, 57]
    manager.addReward(std::move(reward1));
    manager.addReward(std::move(reward2));

    // 3. Game Loop berjalan... Kondisi berubah seiring gameplay [cite: 59]
    std::cout << "\n--------------------------------------------------\n";
    std::cout << "[Gameplay] Pemain memutuskan masuk ke SHOP berikutnya...\n";
    // Memicu event EnterShop [cite: 9, 59]
    manager.handleEvent(TriggerEvent::EnterShop); 

    std::cout << "\n--------------------------------------------------\n";
    std::cout << "[Gameplay] Pemain keluar dari shop dan memulai BLIND berikutnya...\n";
    // Memicu event EnterBlind [cite: 10, 59]
    manager.handleEvent(TriggerEvent::EnterBlind);

    std::cout << "\n=== SIMULASI SELESAI ===\n";
    return 0;
}
