/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIMaker.h"

class FFSMConditionAnyComplete;
class FFSMConditionCheckPathOption;
class FFSMConditionCheckStateTime;
class FFSMConditionECComplete;
class FFSMConditionEventCopyVariable;
class FFSMConditionEventRandomPathOption;
class FFSMConditionEventSetPathOption;
class FFSMConditionEventSetVariable;
class FFSMConditionFalse;
class FFSMConditionOrOperator;
class FFSMConditionRandomRoll;
class FFSMConditionTrue;
class FFSMConditionVariableEqual;
class FFSMConditionVariableGreater;
class FFSMConditionVariableGreaterEqual;
class FFSMConditionVariableLess;
class FFSMConditionVariableLessEqual;
class FFSMConditionVariableNotEqual;
class FGrannyState;
class FGrannyStateMachine;
class FGrannyStateMachineInstance;
class FGrannyTimedTriggerSystem;
class FIMaker;
class FTimedTriggerAction;
class FTimedTriggerEffect;
class FTimedTriggerSound;
class FTimedTriggerStopSound;
class FTimedTriggerTransfer;
class cvActionTrigger;
class cvGrannyStateMachineInstance;
class cvParticleEffectTrigger;
class cvSoundEffectTrigger;
class cvSoundStopTrigger;
class cvTransferEffectTrigger;

/// size ?
/// vtable 0x0a302c20 FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
/// vtable 0x0a302a40 FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
/// vtable 0x0a3029e0 FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
/// vtable 0x0a302bc0 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
/// vtable 0x0a302ce0 FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
/// vtable 0x0a302aa0 FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
/// vtable 0x0a302b00 FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
/// vtable 0x0a302dc0 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
/// vtable 0x0a303060 FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
/// vtable 0x0a302c80 FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
/// vtable 0x0a302b60 FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
/// vtable 0x0a303140 FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
/// vtable 0x0a303000 FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
/// vtable 0x0a302ee0 FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
/// vtable 0x0a302e20 FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
/// vtable 0x0a302f40 FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
/// vtable 0x0a302e80 FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
/// vtable 0x0a302fa0 FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
/// vtable 0x0a3017c0 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
/// vtable 0x0a301760 FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
/// vtable 0x0a2fce00 FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
/// vtable 0x0a2fcfd0 FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
/// vtable 0x0a2fd030 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
/// vtable 0x0a2fd610 FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
/// vtable 0x0a2fd0f0 FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
/// vtable 0x0a2fd5b0 FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
/// vtable 0x0a2fd1b0 FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
/// vtable 0x0a2fd150 FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
/// vtable 0x0a2fd090 FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
template <class T0, class T1, unsigned int T2>
class FTMaker : public FIMaker {
public:
    /// 0x087fc870 FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// deleting 0x08cedb0a FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// deleting 0x08ced0e8 FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// deleting 0x08cecea2 FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// deleting 0x08ced9d0 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// deleting 0x08cee01e FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// deleting 0x08ced33e FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// deleting 0x08ced53c FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// deleting 0x08cee5a8 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// deleting 0x08cefa12 FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// deleting 0x08cedd88 FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// deleting 0x08ced78e FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// deleting 0x08cefe0c FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// deleting 0x08cef72c FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// deleting 0x08ceee7a FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// deleting 0x08cee8ae FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// deleting 0x08cef160 FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// deleting 0x08ceeb94 FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// 0x087fc870 FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// deleting 0x08cef446 FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// 0x087fc870 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// deleting 0x08cbafe0 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// 0x087fc870 FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// deleting 0x08cbaea6 FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// 0x087fc870 FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// deleting 0x087fc88c FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// 0x087fc870 FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// deleting 0x087ff982 FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// 0x087fc870 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// deleting 0x087ffb80 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// 0x087fc870 FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// deleting 0x08860a26 FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// 0x087fc870 FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// deleting 0x087fff7c FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// 0x087fc870 FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// deleting 0x088608ec FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// 0x087fc870 FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// deleting 0x08800378 FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// 0x087fc870 FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// deleting 0x0880017a FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// 0x087fc870 FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    /// deleting 0x087ffd7e FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    virtual ~FTMaker();
    /// 0x087fc8a0 FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// 0x087fc8a0 FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// 0x087fc8a0 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// 0x087fc8a0 FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// 0x087fc8a0 FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// 0x087fc8a0 FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// 0x087fc8a0 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// 0x087fc8a0 FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// 0x087fc8a0 FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// 0x087fc8a0 FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// 0x087fc8a0 FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// 0x087fc8a0 FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// 0x087fc8a0 FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x087fc8c0 FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// 0x087fc8c0 FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// 0x087fc8c0 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// 0x087fc8c0 FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// 0x087fc8c0 FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// 0x087fc8c0 FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// 0x087fc8c0 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// 0x087fc8c0 FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// 0x087fc8c0 FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// 0x087fc8c0 FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// 0x087fc8c0 FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// 0x087fc8c0 FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// 0x087fc8c0 FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08cedb1e FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// 0x08ced0fc FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// 0x08ceceb6 FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// 0x08ced9e4 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// 0x08cee032 FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// 0x08ced352 FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// 0x08ced550 FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// 0x08cee5bc FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// 0x08cefa26 FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// 0x08cedd9c FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// 0x08ced7a2 FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// 0x08cefe20 FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// 0x08cef740 FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// 0x08ceee8e FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// 0x08cee8c2 FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// 0x08cef174 FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// 0x08ceeba8 FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// 0x08cef45a FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// 0x08cbaff4 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// 0x08cbaeba FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// 0x087fc8f6 FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// 0x087ff996 FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// 0x087ffb94 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// 0x08860a3a FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// 0x087fff90 FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// 0x08860900 FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// 0x0880038c FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// 0x0880018e FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// 0x087ffd92 FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// 0x08cedba2 FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// 0x08ced16c FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// 0x08cecf1e FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// 0x08ceda28 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// 0x08cee114 FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// 0x08ced396 FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// 0x08ced5be FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// 0x08cee682 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// 0x08cefaaa FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// 0x08cede2c FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// 0x08ced808 FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// 0x08cefea4 FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// 0x08cef7f8 FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// 0x08ceef46 FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// 0x08cee97a FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// 0x08cef22c FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// 0x08ceec60 FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// 0x08cef512 FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// 0x08cbb038 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// 0x08cbaefe FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// 0x087fc93a FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// 0x087ff9da FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// 0x087ffbd8 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// 0x08860a7e FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// 0x087fffd4 FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// 0x08860944 FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// 0x088003d0 FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// 0x088001d2 FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// 0x087ffdd6 FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x08cedc0c FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// 0x08ced1c2 FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// 0x08cecf6c FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// 0x08ceda52 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// 0x08cee1ea FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// 0x08ced3c0 FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// 0x08ced612 FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// 0x08cee732 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// 0x08cefb14 FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// 0x08cedea2 FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// 0x08ced854 FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// 0x08ceff0e FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// 0x08cef896 FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// 0x08ceefe4 FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// 0x08ceea18 FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// 0x08cef2ca FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// 0x08ceecfe FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// 0x08cef5b0 FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// 0x08cbb062 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// 0x08cbaf28 FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// 0x087fc964 FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// 0x087ffa04 FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// 0x087ffc02 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// 0x08860aa8 FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// 0x087ffffe FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// 0x0886096e FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// 0x088003fa FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// 0x088001fc FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// 0x087ffe00 FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x08cedc12 FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// 0x08ced1c8 FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// 0x08cecf72 FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// 0x08ceda58 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// 0x08cee1f0 FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// 0x08ced3c6 FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// 0x08ced618 FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// 0x08cee738 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// 0x08cefb1a FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// 0x08cedea8 FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// 0x08ced85a FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// 0x08ceff14 FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// 0x08cef89c FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// 0x08ceefea FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// 0x08ceea1e FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// 0x08cef2d0 FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// 0x08ceed04 FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// 0x08cef5b6 FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// 0x08cbb068 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// 0x08cbaf2e FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// 0x087fc96a FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// 0x087ffa0a FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// 0x087ffc08 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// 0x08860aae FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// 0x08800004 FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// 0x08860974 FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// 0x08800400 FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// 0x08800202 FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// 0x087ffe06 FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08cedc3a FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// 0x08ced1f0 FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// 0x08cecf9a FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// 0x08ceda80 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// 0x08cee218 FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// 0x08ced3ee FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// 0x08ced640 FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// 0x08cee760 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// 0x08cefb42 FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// 0x08ceded0 FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// 0x08ced882 FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// 0x08ceff3c FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// 0x08cef8c4 FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// 0x08cef012 FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// 0x08ceea46 FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// 0x08cef2f8 FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// 0x08ceed2c FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// 0x08cef5de FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// 0x08cbb090 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// 0x08cbaf56 FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// 0x087fc992 FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// 0x087ffa32 FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// 0x087ffc30 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// 0x08860ad6 FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// 0x0880002c FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// 0x0886099c FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// 0x08800428 FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// 0x0880022a FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// 0x087ffe2e FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08cedc62 FTMaker<FFSMConditionAnyComplete, FFSMConditionAnyComplete, 36u>
    /// 0x08ced218 FTMaker<FFSMConditionCheckPathOption, FFSMConditionCheckPathOption, 36u>
    /// 0x08cecfc2 FTMaker<FFSMConditionCheckStateTime, FFSMConditionCheckStateTime, 36u>
    /// 0x08cedaa8 FTMaker<FFSMConditionECComplete, FFSMConditionECComplete, 36u>
    /// 0x08cee240 FTMaker<FFSMConditionEventCopyVariable, FFSMConditionEventCopyVariable, 36u>
    /// 0x08ced416 FTMaker<FFSMConditionEventRandomPathOption, FFSMConditionEventRandomPathOption, 36u>
    /// 0x08ced668 FTMaker<FFSMConditionEventSetPathOption, FFSMConditionEventSetPathOption, 36u>
    /// 0x08cee788 FTMaker<FFSMConditionEventSetVariable, FFSMConditionEventSetVariable, 36u>
    /// 0x08cefb6a FTMaker<FFSMConditionFalse, FFSMConditionFalse, 36u>
    /// 0x08cedef8 FTMaker<FFSMConditionOrOperator, FFSMConditionOrOperator, 36u>
    /// 0x08ced8aa FTMaker<FFSMConditionRandomRoll, FFSMConditionRandomRoll, 36u>
    /// 0x08ceff64 FTMaker<FFSMConditionTrue, FFSMConditionTrue, 36u>
    /// 0x08cef8ec FTMaker<FFSMConditionVariableEqual, FFSMConditionVariableEqual, 36u>
    /// 0x08cef03a FTMaker<FFSMConditionVariableGreater, FFSMConditionVariableGreater, 36u>
    /// 0x08ceea6e FTMaker<FFSMConditionVariableGreaterEqual, FFSMConditionVariableGreaterEqual, 36u>
    /// 0x08cef320 FTMaker<FFSMConditionVariableLess, FFSMConditionVariableLess, 36u>
    /// 0x08ceed54 FTMaker<FFSMConditionVariableLessEqual, FFSMConditionVariableLessEqual, 36u>
    /// 0x08cef606 FTMaker<FFSMConditionVariableNotEqual, FFSMConditionVariableNotEqual, 36u>
    /// 0x08cbb0b8 FTMaker<FGrannyStateMachine, FGrannyStateMachine, 36u>
    /// 0x08cbaf7e FTMaker<FGrannyStateMachineInstance, FGrannyStateMachineInstance, 36u>
    /// 0x087fc9ba FTMaker<FGrannyStateMachineInstance, cvGrannyStateMachineInstance, 36u>
    /// 0x087ffa5a FTMaker<FGrannyTimedTriggerSystem, FGrannyTimedTriggerSystem, 34u>
    /// 0x087ffc58 FTMaker<FTimedTriggerAction, cvActionTrigger, 34u>
    /// 0x08860afe FTMaker<FTimedTriggerEffect, FTimedTriggerEffect, 34u>
    /// 0x08800054 FTMaker<FTimedTriggerEffect, cvParticleEffectTrigger, 34u>
    /// 0x088609c4 FTMaker<FTimedTriggerSound, FTimedTriggerSound, 34u>
    /// 0x08800450 FTMaker<FTimedTriggerSound, cvSoundEffectTrigger, 34u>
    /// 0x08800252 FTMaker<FTimedTriggerStopSound, cvSoundStopTrigger, 34u>
    /// 0x087ffe56 FTMaker<FTimedTriggerTransfer, cvTransferEffectTrigger, 34u>
    virtual UNK_RET vf_09(UNK_PARAMS);
};
