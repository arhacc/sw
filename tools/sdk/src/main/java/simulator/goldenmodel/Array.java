package xpu.sw.tools.sdk.simulator.goldenmodel;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;

import java.util.Stack;

public class Array {
    ArchitectureImplementation architectureImplementation;
    long []mCodeArrayMem;
    long [][]mDataArrayMem;
    long []mFeatureArrayMem;

    Stack<boolean[]> mWhere;
    int mNumCells;

    public Array(Context _context, ArchitectureImplementation _architectureImplementation) {
        architectureImplementation = _architectureImplementation;
        mFeatureArrayMem = new long[_architectureImplementation.getMemFeatureSizeLog()];
        mCodeArrayMem = new long[1 << _architectureImplementation.getMemCodeArraySizeLog()];
        mNumCells = _architectureImplementation.getNCells();
        mDataArrayMem = new long[1 << _architectureImplementation.getMemDataArraySizeLog()][mNumCells];
        mWhere = new Stack<>();
    }

    public int getNumCells() {
        return mNumCells;
    }

    public long[] getCodeArrayMem() {
        return mCodeArrayMem;
    }
    public long[][] getDataArrayMem() { return mDataArrayMem; }
    public long[] getFeaturesArrayMem() { return mFeatureArrayMem; }

    public boolean runOnce(int StartAddress) {
        return false;
    }
}
