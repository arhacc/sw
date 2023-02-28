package xpu.sw.tools.sdk.simulator.goldenmodel;

import java.util.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.isa.builders.*;
import xpu.sw.tools.sdk.common.utils.switcher.*;


public class Controller {
    InstructionBuilder instructionBuilder;
    Switcher switcher;
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
        String _currentArhCode = 
        // TODO: goldenmodel needs to be instantiated with a arhCode parameter
        _context.getArchitectureImplementations().getDefault().getName();

        mCodeControllerMem = new long[1 << _context.getArchitectureImplementations().getArchitecture(_currentArhCode).getMemCodeControllerSizeLog()];

        //mDataControllerMem = new Long[1 << _context.getMemDataControllerSizeLog()];
        mDataControllerMem = new long[1 << _context.getArchitectureImplementations().getArchitecture(_currentArhCode).getMemCodeControllerSizeLog()];
        instructionBuilder = new InstructionBuilder(_context);
        switcher = new Switcher(_context);
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        switcher.addCaseCommand(Opcode.ADD.getData(), new Command() {
            public void execute(int _operandValue) {
                mZero = computeZero(mAccumulator + _operandValue);
                mCarry = computeCarry(mAccumulator + _operandValue);
                mAccumulator = computeAccumulator(mAccumulator + _operandValue);
            }
        });

        switcher.addCaseCommand(Opcode.ADDC.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator + _operandValue + mCarry);
                    mCarry = computeCarry(mAccumulator + _operandValue + mCarry);
                    mAccumulator = computeAccumulator(mAccumulator + _operandValue + mCarry);
            }
        });

        switcher.addCaseCommand(Opcode.SUB.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator - _operandValue);
                    mCarry = computeCarry(mAccumulator - _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator - _operandValue);
            }
        });

        switcher.addCaseCommand(Opcode.RSUB.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue - mAccumulator);
                    mCarry = computeCarry(_operandValue - mAccumulator);
                    mAccumulator = computeAccumulator(_operandValue - mAccumulator);
            }
        });

        switcher.addCaseCommand(Opcode.SUBC.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator - _operandValue - mCarry);
                    mCarry = computeCarry(mAccumulator - _operandValue - mCarry);
                    mAccumulator = computeAccumulator(mAccumulator - _operandValue - mCarry);
            }
        });

        switcher.addCaseCommand(Opcode.RSUBC.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue - mAccumulator - mCarry);
                    mCarry = computeCarry(_operandValue - mAccumulator - mCarry);
                    mAccumulator = computeAccumulator(_operandValue - mAccumulator - mCarry);
            }
        });

        switcher.addCaseCommand(Opcode.MULT.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator * _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator * _operandValue);
            }
        });

        switcher.addCaseCommand(Opcode.BWAND.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator & _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator & _operandValue);
            }
        });

        switcher.addCaseCommand(Opcode.BWOR.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator | _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator | _operandValue);
            }
        });

        switcher.addCaseCommand(Opcode.BWXOR.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(mAccumulator ^ _operandValue);
                    mAccumulator = computeAccumulator(mAccumulator ^ _operandValue);
            }
        });

        switcher.addCaseCommand(Opcode.LOAD.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue);
                    mAccumulator = computeAccumulator(_operandValue);
            }
        });

        switcher.addCaseCommand(Opcode.STORE.getData(), new Command() {
            public void execute(int _operandValue) {
                    //mAccumulator = computeAccumulator(mAccumulator);
            }
        });

        switcher.addCaseCommand(Opcode.ADDRINC.getData(), new Command() {
            public void execute(int _operandValue) {
                    mDataAddress = mDataAddress + _operandValue;
            }
        });

        switcher.addCaseCommand(Opcode.COMPARE.getData(), new Command() {
            public void execute(int _operandValue) {
                    mCarry = ((mAccumulator - _operandValue) > 0) ? 1 : 0;
            }
        });

        switcher.addCaseCommand(Opcode.STACK_STORE_POP.getData(), new Command() {
            public void execute(int _operandValue) {
                    _operandValue = mAccumulator;
                    mAccumulator = mStack.pop();
            }
        });

        switcher.addCaseCommand(Opcode.STACK_PUSH_LOAD.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue);
                    mAccumulator = _operandValue;
                    mStack.push(mAccumulator);
            }
        });

        switcher.addCaseCommand(Opcode.SWAP_MEMACC.getData(), new Command() {
            public void execute(int _operandValue) {
                    mZero = computeZero(_operandValue);
                    mAccumulator = _operandValue;
                    mStack.push(mAccumulator);
                    _operandValue = mAccumulator; // CLN: why doing this here ?
            }
        });

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
        Opcode _opcode = instructionBuilder.getOpcode((int) ((instruction >> 11) & 0x1F));
        int OperandSel = (int) ((instruction >> 8) & 7);
        int Scalar = (int) ((instruction >> 0) & 0xFF);

        int _operandValue = 0;

        switch (OperandSel) {

            case Operand.OP_VAL:
                _operandValue = Scalar;
                break;
            case Operand.OP_MAB:
                _operandValue = (int)mDataControllerMem[Scalar];
                break;
            case Operand.OP_MRL:
                _operandValue = (int)mDataControllerMem[mDataAddress + Scalar];
                break;
            case Operand.OP_MRI:
                mDataAddress += Scalar;
                _operandValue = (int)mDataControllerMem[mDataAddress];
                break;
            case Operand.OP_COP:
                //mDataAddress += Scalar;
                //_operandValue = (int)mDataControllerMem[mDataAddress];
                break;
/*            case Operand.OP_STK:
                //mDataAddress += Scalar;
                //_operandValue = (int)mDataControllerMem[mDataAddress];
                break;*/
            case Operand.OP_MRC:
                //mDataAddress += Scalar;
                //_operandValue = (int)mDataControllerMem[mDataAddress];
                break;
            default:
                break;
        }
        if (OperandSel != Operand.OP_CTL){
            switcher.on(_opcode.getData(), _operandValue);            
        } else {
            // no-operand opcodes
            if(_opcode.equals(Opcode.JMP)) {
                int jumpType = Scalar & 0x0F;
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
