/// origin guess
#pragma once

/// size ?
class cvAudioSimSystem {
public:
    /// 0x0878dd98
    cvAudioSimSystem();
    /// 0x0878de1e
    UNK_RET Startup();
    /// 0x0878de82
    UNK_RET SubscribeToEvents();
    /// 0x0878de84
    UNK_RET ActivateSims();
    /// 0x0878dec0
    UNK_RET Update(float a1);
    /// 0x0878df50
    UNK_RET Shutdown();
    /// 0x0878df64
    UNK_RET UnSubscribeFromEvents();
    /// 0x0878df66
    UNK_RET DeactivateSims();
    /// 0x0878dfb0
    UNK_RET RefreshGameStates();
    /// 0x0878e038
    UNK_RET StartupGame();
    /// 0x0878e058
    UNK_RET ShutdownMainGame();
    /// 0x0878e078
    UNK_RET CreateHandle(char const* a1);
    /// 0x0878e0e2
    UNK_RET HandleToName(unsigned int a1);
    /// 0x0878e110
    UNK_RET HandleNameContainsMacros(unsigned int a1);
};
