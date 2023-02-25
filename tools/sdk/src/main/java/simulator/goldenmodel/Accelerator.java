package xpu.sw.tools.sdk.simulator.goldenmodel;

import org.jline.utils.Log;
import xpu.sw.tools.sdk.common.context.Context;
import xpu.sw.tools.sdk.common.isa.Operand;
import xpu.sw.tools.sdk.common.isa.Operation;

public class Accelerator {

    /*parameter
    n = 32 ,// word size
    x = 10 ,// index size −> 2 ˆ x = 1024 cells
    v = 11 ,// vector memory address size −> 2048 1024− component vectors
    s = 9 , // scalar memory address size −> 512 32−bit scalars
    p = 8 , // primitive memory address size −> 256 pairs of instructions
    c = 8 , // value size in instruction
    a = 5   // (sizeof activation counter −> 32 embedded WHEREs)
     */
    //public static final int ARRAY_N = 32;
    //public static final int ARRAY_C = 8;
    //public static final int ARRAY_A = 5;

    //public static final int ARRAY_WORD_MASK = (1 << ARRAY_N) - 1;
    //public static final int ARRAY_IMM_VALUE_MASK = (1 << ARRAY_C) - 1;
    //public static final int ARRAY_WHERE_STACK_SIZE = (1 << ARRAY_A);
    Array mArray;
    Controller mController;
    Long mFeatures;

    public Accelerator(Context _context) {
        mArray = new Array(_context);
        mController = new Controller(_context);
    }

    static long getSinstr(int opcode, int operand, int scalar) {
        return (opcode << 11L) + (operand << 8L) + (scalar) << 0L;
    }

    static long getCAinstr(long cInstruction, long aInstruction) {
        return (cInstruction << 16L) + (aInstruction << 0L);
    }

    public static long NO_OP() {
        return getSinstr(Operation.JMP.getIntData(), Operand.VAL.getIntData(), 0);
    }

    public static void testAccelerator(Context _context) {
        Accelerator acc = new Accelerator(_context);
        long payload[] = new long[]{
            getCAinstr(getSinstr(Operation.LOAD.getIntData(), Operand.VAL.getIntData(), 0), NO_OP()),
            getCAinstr(NO_OP(), NO_OP()),};

        try {
            acc.uploadCode(0, payload);
            acc.run(0);
            //acc.checkCAccumulator(0);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private boolean transferFeatures(int _address,
            long[] _destination, long[] _source) {
        if (_address + _source.length > _destination.length) {
            return false;
        }

        for (int i = 0; i < _source.length; i++) {
            _destination[i] = _source[i];
            mFeatures = _source[i];
        }

        return true;
    }

    private boolean transferData(long[] _destination, int _destinationOffset, long[] _source, int _sourceOffset,
            int _length) {
        if (_length + _destinationOffset > _destination.length) {
            return false;
        }
        if (_length + _sourceOffset > _source.length) {
            return false;
        }

        for (int i = 0; i < _length; i++) {
            _destination[i + _destinationOffset] = _source[i + _sourceOffset];
        }
        return true;
    }

    private boolean transferCode(int _address,
            long[] _destinationLeft, long[] _destinationRight,
            long[] _source) {
        if (_address + _source.length > _destinationLeft.length) {
            return false;
        }
        if (_address + _source.length > _destinationRight.length) {
            return false;
        }

        for (int i = 0; i < _source.length; i++) {
            _destinationLeft[_address + i] = (_source[i] >> 31) & 0xFFFFFFFF;
            _destinationRight[_address + i] = _source[i] & 0xFFFFFFFF;
        }

        return true;
    }

    public boolean uploadCode(int _address, long[] _payload) {
        return transferCode(_address, mController.getCodeControllerMem(), mArray.getCodeArrayMem(), _payload);
    }

    public boolean uploadData(int _address, long[] payload) {
        boolean result = true;
        int payloadLength = payload.length;
        int offset = 0;

        while (payloadLength > 0) {
            result &= transferData(mArray.getDataArrayMem()[_address], 0, payload, offset, mArray.getNumCells());
            payloadLength -= mArray.getNumCells();
            offset += mArray.getNumCells();
            _address++;
        }
        return result;
    }

    public long[] downloadData(int _address, int _size) {
        long[] result = new long[_size];
        int length = _size;
        int offset = 0;

        while (length > 0) {
            if (false == transferData(result, offset, mArray.getDataArrayMem()[_address], 0, mArray.getNumCells())) {
                Log.error("Could not transfer data");
            }
            length -= mArray.getNumCells();
            offset += mArray.getNumCells();
            _address++;
        }
        return result;
    }

    public boolean uploadFeatures(int _address, long[] payload) {
        boolean result = true;
        int payloadLength = payload.length;
        int offset = 0;

        result &= transferFeatures(_address, mArray.getFeaturesArrayMem(), payload);
        return result;
    }

    public long[] downloadFeatures(int _address, int _size) {
        long[] result = new long[_size];
        int length = _size;
        int offset = 0;

        if (false == transferFeatures(_address, result, mArray.getFeaturesArrayMem())) {
            Log.error("Could not transfer features");
        }
        return result;
    }

    public void run(int StartAddress) throws Exception {
        boolean controllerHalt = mController.runOnce(StartAddress);
        boolean arrayHalt = mArray.runOnce(StartAddress);
        while (controllerHalt == false || arrayHalt == false) {
            controllerHalt = mController.runStep();
            //arrayHalt = mArray.runStep();
        }
    }
}
