package xpu.sw.tools.sdk.simulator.goldenmodel;

import java.util.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.isa.builders.*;
import xpu.sw.tools.sdk.common.utils.switcher.*;


public class Controller {
//    InstructionBuilder instructionBuilder;
    OpcodeBuilder opcodeBuilder;
    OperandBuilder operandBuilder;

    Switcher opcodeSwitcher;
    Switcher operandSwitcher;

    int operandDataCTL;
    int opcodeDataJMP;
    int operandValue;
    int scalar;

    long []mCodeControllerMem;
    long []mDataControllerMem;

    int mZero;
    int mCarry;
    int mAccumulator;

    int mDataAddress;
    int mCodeAddress;
    Stack<Integer> mStack;

    int mMask = 0xFFFFFFFF;
    int mMaskbits = 32;

    public Controller(Context _context, ArchitectureImplementation _architectureImplementation) {
        String _currentArchitectureId = 
        // TODO: goldenmodel needs to be instantiated with a architectureId parameter
        _context.getArchitectureImplementations().getDefault().getName();

        mCodeControllerMem = new long[1 << _context.getArchitectureImplementations().getArchitecture(_currentArchitectureId).getMemCodeControllerSize()];

        //mDataControllerMem = new Long[1 << _context.getMemDataControllerSizeLog()];
        mDataControllerMem = new long[1 << _context.getArchitectureImplementations().getArchitecture(_currentArchitectureId).getMemCodeControllerSize()];
//        instructionBuilder = new InstructionBuilder(_context, _currentArchitectureId);
        opcodeBuilder = new OpcodeBuilder(_context, _currentArchitectureId);
        operandBuilder = new OperandBuilder(_context, _currentArchitectureId);
        opcodeSwitcher = new Switcher(_context);
        operandSwitcher = new Switcher(_context);
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("ADD").getData(), new Command() {
            public void execute(int _operandValue) {
                mZero = computeZero(mAccumulator + _operandValue);
                mCarry = computeCarry(mAccumulator + _operandValue);
                mAccumulator = computeAccumulator(mAccumulator + _operandValue);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("ADDC").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator + _operandValue + mCarry);
                    mCarry = computeCarry(mAccumulator + _operandValue + mCarry);
                    mAccumulator = computeAccumulator(mAccumulator + _operandValue + mCarry);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("SUB").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator - _operandValue);
                    mCarry = computeCarry(mAccumulator - _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator - _operandValue);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("RSUB").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue - mAccumulator);
                    mCarry = computeCarry(_operandValue - mAccumulator);
                    mAccumulator = computeAccumulator(_operandValue - mAccumulator);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("SUBC").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator - _operandValue - mCarry);
                    mCarry = computeCarry(mAccumulator - _operandValue - mCarry);
                    mAccumulator = computeAccumulator(mAccumulator - _operandValue - mCarry);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("RSUBC").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue - mAccumulator - mCarry);
                    mCarry = computeCarry(_operandValue - mAccumulator - mCarry);
                    mAccumulator = computeAccumulator(_operandValue - mAccumulator - mCarry);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("MULT").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator * _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator * _operandValue);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("BWAND").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator & _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator & _operandValue);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("BWOR").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator | _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator | _operandValue);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("BWXOR").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator ^ _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator ^ _operandValue);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("LOAD").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue);
                    mAccumulator = computeAccumulator(_operandValue);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("STORE").getData(), new Command() {
            public void execute(int _operandValue) {
                    //mAccumulator = computeAccumulator(mAccumulator);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("ADDRINC").getData(), new Command() {
            public void execute(int _operandValue) {
                    mDataAddress = mDataAddress + _operandValue;
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("COMPARE").getData(), new Command() {
            public void execute(int _operandValue) {
                    mCarry = ((mAccumulator - _operandValue) > 0) ? 1 : 0;
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("STACK_STORE_POP").getData(), new Command() {
            public void execute(int _operandValue) {
                    _operandValue = mAccumulator;
                    mAccumulator = mStack.pop();
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("STACK_PUSH_LOAD").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue);
                    mAccumulator = _operandValue;
                    mStack.push(mAccumulator);
            }
        });

        opcodeSwitcher.addCaseCommand(opcodeBuilder.get("SWAP_MEMACC").getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue);
                    mAccumulator = _operandValue;
                    mStack.push(mAccumulator);
                    _operandValue = mAccumulator; // CLN: why doing this here ?
            }
        });

        opcodeDataJMP = opcodeBuilder.get("JMP").getData();
        operandDataCTL = operandBuilder.get("CTL").getData();

                /*case Opcode.ROTATE_LOCAL:
                    int rotatemode = (_operandValue >> 5) & 3;
                    int rotateamount = (_operandValue & 0x0F);
                    switch (rotatemode) {
                        case 0: mAccumulator = computeAccumulator(mAccumulator >> rotateamount); break;
                        case 1: mAccumulator = computeAccumulator(mAccumulator << rotateamount); break;

                        // CLN: who is n ? ((acc >> op[4:0])|(acc << (n - op[4:0]))
                        //case 2: mAccumulator = mAccumulator >> rotateamount; break;

                        // CLN: why is n ? ((acc << op[4:0])|(acc >> (n - op[4:0]))
                        //case 3: mAccumulator = mAccumulator >> rotateamount; break;
                        default: break;
                    }
                    break;*/



        operandSwitcher.addCaseCommand(operandBuilder.get("val").getData(), new Command() {
            public void execute(int _operandValue) {
                operandValue = scalar;
            }
        });
        operandSwitcher.addCaseCommand(operandBuilder.get("mab").getData(), new Command() {
            public void execute(int _operandValue) {
                operandValue = (int)mDataControllerMem[scalar];
            }
        });
        operandSwitcher.addCaseCommand(operandBuilder.get("mrl").getData(), new Command() {
            public void execute(int _operandValue) {
                operandValue = (int)mDataControllerMem[mDataAddress + scalar];
            }
        });
        operandSwitcher.addCaseCommand(operandBuilder.get("mri").getData(), new Command() {
            public void execute(int _operandValue) {
                mDataAddress += scalar;
                operandValue = (int)mDataControllerMem[mDataAddress];
            }
        });
        operandSwitcher.addCaseCommand(operandBuilder.get("cop").getData(), new Command() {
            public void execute(int _operandValue) {
                //mDataAddress += Scalar;
                //_operandValue = (int)mDataControllerMem[mDataAddress];
            }
        });
        operandSwitcher.addCaseCommand(operandBuilder.get("stk").getData(), new Command() {
            public void execute(int _operandValue) {
/*            case Operand.OP_STK:
                //mDataAddress += Scalar;
                //_operandValue = (int)mDataControllerMem[mDataAddress];
                break;*/
            }
        });
        operandSwitcher.addCaseCommand(operandBuilder.get("mrc").getData(), new Command() {
            public void execute(int _operandValue) {
                //mDataAddress += Scalar;
                //_operandValue = (int)mDataControllerMem[mDataAddress];
            }
        });
    }

//-------------------------------------------------------------------------------------
    public long[] getCodeControllerMem() {
        return mCodeControllerMem;
    }

    public long[] getDataControllerMem() {
        return mDataControllerMem;
    }

    public int computeZero(long number) {
        mZero = ((int) ~(number & mMask)) & 1;
        return mZero;
    }

    public int computeCarry(long number) {
        mCarry = number > mMask ? 1 : 0;
        return mCarry;
    }

    public int computeAccumulator(long number) {
        mAccumulator = (int) (number & mMask);
        return mAccumulator;
    }

    public boolean checkState(int Zero, int Carry, int Accumulator) {
        return (mZero == Zero) &&
                    (mCarry == Carry) &&
                        (mAccumulator == Accumulator);
    }

    public boolean runStep() throws Exception {
        long instruction = mCodeControllerMem[mCodeAddress];
        int _opcodeData = (int) ((instruction >> 11) & 0x1F);
        int _operandSel = (int) ((instruction >> 8) & 7);
        scalar = (int) ((instruction >> 0) & 0xFF);

//        int _operandValue = 0;


        opcodeSwitcher.on(_operandSel, scalar);
        if (_operandSel != operandDataCTL){
            operandSwitcher.on(_opcodeData, operandValue);            
        } else {
            // no-operand opcodes
            if(_opcodeData == opcodeDataJMP) {
                int jumpType = scalar & 0x0F;
                switch (jumpType) {
                    //case 0: mCodeAddress +=
                }
            }
        }

        mCodeAddress++;
        return true;
    }

    public boolean runOnce(int startAddress) throws Exception {
        mCodeAddress = startAddress;
        return runStep();
    }
}
