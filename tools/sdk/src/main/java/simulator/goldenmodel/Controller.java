package xpu.sw.tools.sdk.simulator.goldenmodel;

import xpu.sw.tools.sdk.common.context.Context;
import xpu.sw.tools.sdk.common.isa.Opcodes;
import xpu.sw.tools.sdk.common.isa.Operation;
import xpu.sw.tools.sdk.common.isa.RightOperandSelect;

import java.util.Stack;

public class Controller {
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

    public Controller(Context _context) {
        mCodeControllerMem = new long[1 << _context.getMemCodeControllerSizeLog()];

        //mDataControllerMem = new Long[1 << _context.getMemDataControllerSizeLog()];
        mDataControllerMem = new long[1 << _context.getMemCodeControllerSizeLog()];
    }

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
        int Opcode = (int) ((instruction >> 11) & 0x1F);
        int OperandSel = (int) ((instruction >> 8) & 7);
        int Scalar = (int) ((instruction >> 0) & 0xFF);

        int OperandValue = 0;
        switch (OperandSel) {
            case RightOperandSelect.VAL:
                OperandValue = Scalar;
                break;
            case RightOperandSelect.MAB:
                OperandValue = (int)mDataControllerMem[Scalar];
                break;
            case RightOperandSelect.MRL:
                OperandValue = (int)mDataControllerMem[mDataAddress + Scalar];
                break;
            case RightOperandSelect.MRI:
                mDataAddress += Scalar;
                OperandValue = (int)mDataControllerMem[mDataAddress];
                break;
            case RightOperandSelect.COP:
                //mDataAddress += Scalar;
                //OperandValue = (int)mDataControllerMem[mDataAddress];
                break;
            case RightOperandSelect.STK:
                //mDataAddress += Scalar;
                //OperandValue = (int)mDataControllerMem[mDataAddress];
                break;
            case RightOperandSelect.MRC:
                //mDataAddress += Scalar;
                //OperandValue = (int)mDataControllerMem[mDataAddress];
                break;
            default:
                break;
        }

        if (OperandSel != RightOperandSelect.CTL)
            switch (Opcode) {
                case Opcodes.ADD:
                    mZero = computeZero(mAccumulator + OperandValue);
                    mCarry = computeCarry(mAccumulator + OperandValue);
                    mAccumulator = computeAccumulator(mAccumulator + OperandValue);
                    break;

                case Opcodes.ADDC:
                    mZero = computeZero(mAccumulator + OperandValue + mCarry);
                    mCarry = computeCarry(mAccumulator + OperandValue + mCarry);
                    mAccumulator = computeAccumulator(mAccumulator + OperandValue + mCarry);
                    break;

                case Opcodes.SUB:
                    mZero = computeZero(mAccumulator - OperandValue);
                    mCarry = computeCarry(mAccumulator - OperandValue);
                    mAccumulator = computeAccumulator(mAccumulator - OperandValue);
                    break;

                case Opcodes.RSUB:
                    mZero = computeZero(OperandValue - mAccumulator);
                    mCarry = computeCarry(OperandValue - mAccumulator);
                    mAccumulator = computeAccumulator(OperandValue - mAccumulator);
                    break;

                case Opcodes.SUBC:
                    mZero = computeZero(mAccumulator - OperandValue - mCarry);
                    mCarry = computeCarry(mAccumulator - OperandValue - mCarry);
                    mAccumulator = computeAccumulator(mAccumulator - OperandValue - mCarry);
                    break;

                case Opcodes.RSUBC:
                    mZero = computeZero(OperandValue - mAccumulator - mCarry);
                    mCarry = computeCarry(OperandValue - mAccumulator - mCarry);
                    mAccumulator = computeAccumulator(OperandValue - mAccumulator - mCarry);
                    break;

                case Opcodes.MULT:
                    mZero = computeZero(mAccumulator * OperandValue);
                    mAccumulator = computeAccumulator(mAccumulator * OperandValue);
                    break;

                case Opcodes.BWAND:
                    mZero = computeZero(mAccumulator & OperandValue);
                    mAccumulator = computeAccumulator(mAccumulator & OperandValue);
                    break;

                case Opcodes.BWOR:
                    mZero = computeZero(mAccumulator | OperandValue);
                    mAccumulator = computeAccumulator(mAccumulator | OperandValue);
                    break;

                case Opcodes.BWXOR:
                    mZero = computeZero(mAccumulator ^ OperandValue);
                    mAccumulator = computeAccumulator(mAccumulator ^ OperandValue);
                    break;

                case Opcodes.LOAD:
                    mZero = computeZero(OperandValue);
                    mAccumulator = computeAccumulator(OperandValue);
                    break;

                case Opcodes.STORE:
                    //mAccumulator = computeAccumulator(mAccumulator);
                    break;

                case Opcodes.ADDRINC:
                    mDataAddress = mDataAddress + OperandValue;
                    break;

                case Opcodes.COMPARE:
                    mCarry = ((mAccumulator - OperandValue) > 0) ? 1 : 0;
                    break;

                case Opcodes.STACK_STORE_POP:
                    OperandValue = mAccumulator;
                    mAccumulator = mStack.pop();
                    break;

                case Opcodes.STACK_PUSH_LOAD:
                    mZero = computeZero(OperandValue);
                    mAccumulator = OperandValue;
                    mStack.push(mAccumulator);
                    break;

                case Opcodes.SWAP_MEMACC:
                    mZero = computeZero(OperandValue);
                    mAccumulator = OperandValue;
                    mStack.push(mAccumulator);
                    OperandValue = mAccumulator; // CLN: why doing this here ?
                    break;

                /*case Opcodes.ROTATE_LOCAL:
                    int rotatemode = (OperandValue >> 5) & 3;
                    int rotateamount = (OperandValue & 0x0F);
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
                default:
                    throw new Exception("Unknown opcode: " + Opcode);
            }
        else {
            // no-operand opcodes
            switch (Opcode) {
                case Opcodes.JMP: {
                    int jumpType = Scalar & 0x0F;
                    switch (jumpType) {
                        //case 0: mCodeAddress +=
                    }
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
