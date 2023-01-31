package xpu.sw.tools.sdk.simulator.goldenmodel;

import xpu.sw.tools.sdk.common.context.Context;

import java.util.Stack;

public class Array {
    long []mCodeArrayMem;
    long [][]mDataArrayMem;
    long []mFeatureArrayMem;

    Stack<boolean[]> mWhere;
    int mNumCells;

    public Array(Context _context) {
        mFeatureArrayMem = new long[_context.getMemFeatureSizeLog()];
        mCodeArrayMem = new long[1 << _context.getMemCodeArraySizeLog()];
        mNumCells = _context.getNCells();
        mDataArrayMem = new long[1 << _context.getMemDataArraySizeLog()][mNumCells];
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
