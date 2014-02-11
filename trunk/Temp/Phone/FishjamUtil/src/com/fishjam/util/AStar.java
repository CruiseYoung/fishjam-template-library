package com.fishjam.util;

import java.util.ArrayList;

import android.util.Log;

/**************************************************************************************************************************************
 * ��Ϸ��ͼ�༭��( http://download.csdn.net/download/xys289187120/3452259 )
 *   mappy(mapwin) -- ʹ�� playback library ��װ�غ���ʾFMP��ͼ�ļ�
 *   tiled
 * ���
 *   
 * ��ͼ�༭�� -- http://fc00.deviantart.net/fs70/f/2011/034/c/c/tilemap_editor_b55_pokemon_by_digibody-d36t3aq.swf
 *
 * A���㷨 -- ��ͼ��ƽ���ϣ��ж���ڵ��·����������ͨ���ɱ����㷨
 *   http://blog.sina.com.cn/s/blog_53a5865c0102dycw.html
 *     ��ʵ���� open-list��close-listʹ�õ���ȫ�Ǿ�̬���飬��������ջ��ջ�ĸ���������õ�·�����Ǵ��յ�ָ������
 *     
 ***************************************************************************************************************************************/

/*
 * 
AStar asr = new AStar(map, start_x, start_y, end_x, end_y);
PosInfo[] result = asr.getResult();
if(result != null) { success } 
*/

//�˷���Ѱ·
public class AStar {
	private final static String TAG = AStar.class.getSimpleName();
	
	public class PosInfo {
		public PosInfo(int iRow, int iCol, int iDir) {
			this.row = iRow;
			this.col = iCol;
			this.dir = iDir;
		}

		public PosInfo(PosInfo posInfo) {
			this.row = posInfo.row;
			this.col = posInfo.col;
			this.dir = posInfo.dir;
		}

		public int row;
		public int col;
		public int dir;
	}

	private int[][] mMap; // ��ͼ����0��ʾ��ͨ����1��ʾ����ͨ��
	private int mMapRowCount; 	// ��ͼ���
	private int mMapColumnCount; 				// ��ͼ�߶�
	private int mStartRow; 		// �������
	private int mStartCol; 	// �������
	private int mEndRow; 		// �յ�����
	private int mEndCol; 	// �յ�����
	private boolean mEnable8Way;		//�Ƿ�����8����Ѱ·��false��ʾ4����
	
	private boolean mCloseList[][]; // �ر��б�
	public int mOpenList[][][]; // ���б�
	private int mOpenListLength;

	private static final int EXIST = 1;
	private static final int NOT_EXIST = 0;

	private static final int ISEXIST = 0;
	private static final int EXPENSE = 1; // ����Ĵ���
	private static final int DISTANCE = 2; // ����Ĵ���
	private static final int COST = 3; // ���ĵ��ܴ���
	private static final int FATHER_DIR = 4; // ���ڵ�ķ���

	public static final int DIR_NULL = 0;
	public static final int DIR_DOWN = 1; // ������
	public static final int DIR_UP = 2; // ������
	public static final int DIR_LEFT = 3; // ������
	public static final int DIR_RIGHT = 4; // ������
	public static final int DIR_UP_LEFT = 5;
	public static final int DIR_UP_RIGHT = 6;
	public static final int DIR_DOWN_LEFT = 7;
	public static final int DIR_DOWN_RIGHT = 8;

	private static final int ASTAR_DEPTH = 5000; // �㷨Ƕ�����
	private boolean isFound; // �Ƿ��ҵ�·��

	public AStar(int[][] map, int startRow, int startColumn, int endRow, int endColumn, boolean bEnable8Way) {
		mStartRow = startRow;
		mStartCol = startColumn;
		mEndRow = endRow;
		mEndCol = endColumn;
		mEnable8Way = bEnable8Way;
		
		mMap = map;
		mMapRowCount = map.length;
		mMapColumnCount = map[0].length;
		
		Log.e(TAG, "AStar, Row=" + mMapRowCount +",Column=" + mMapColumnCount 
				+",canPasStart[" + mMap[mStartRow][mStartCol] + "]=" + checkPass(mStartRow, mStartCol)
				+",canPasGoal[" + mMap[mEndRow][mEndCol] + "]=" + checkPass(mEndRow, mEndCol)
		);
		
		initCloseList();
		initOpenList(mEndRow, mEndCol);
	}

	// �õ���ͼ����һ�������ֵ
	private int getMapExpense(int row, int col, int dir) {
		if (dir < 5) {
			return 10;
		} else {
			return 14;
		}
	}

	// �õ����������ֵ
	private int getDistance(int sRow, int sCol, int eRow, int eCol) {
		return 10 * (Math.abs(sRow - eRow) + Math.abs(sCol - eCol));
	}

	// �õ�����������Ӵ�ʱ��������ֵ
	private int getCost(int iRow, int iCol) {
		return mOpenList[iRow][iCol][COST];
	}

	// ��ʼѰ·
	public void searchPath() {
		Log.e(TAG, "isCanPass Start=" + isCanPass(mStartRow, mStartCol));//+", Reason=" + sTmpReason);
		Log.e(TAG, "isCanPass Goal=" + isCanPass(mEndRow, mEndCol));// +", Reason=" + sTmpReason);
		
		if (isCanPass(mStartRow, mStartCol) && isCanPass(mEndRow, mEndCol)) {
			addOpenList(mStartRow, mStartCol);
			aStar(mStartRow, mStartCol);
		}
		String strFound = "" + isFound;
		Log.e(TAG, String.format("searchPath, [%1$d,%2$d] -> [%3$d,%4$d], isFound=%5$s", mStartRow, mStartCol, mEndRow, mEndCol, 
				strFound));
	}

	// Ѱ·
	private void aStar(int row, int col) {
		// �����㷨���
		for (int depth = 0; depth < ASTAR_DEPTH; depth++) {
			if (((row == mEndRow) && (col == mEndCol))) {
				Log.i(TAG, "find AStart, depth=" + depth);
				isFound = true;
				return;
			} else if ((mOpenListLength == 0)) {
				Log.i(TAG, "can not find AStart, depth=" + depth);
				isFound = false;
				return;
			}

			removeOpenList(row, col);
			addCloseList(row, col);

			// �õ���Χ�ܹ����ߵĵ�
			addNewOpenList(row, col, row, col + 1, DIR_UP);
			addNewOpenList(row, col, row, col - 1, DIR_DOWN);
			addNewOpenList(row, col, row - 1, col, DIR_RIGHT);
			addNewOpenList(row, col, row + 1, col, DIR_LEFT);
			if (mEnable8Way) {
				addNewOpenList(row, col, row + 1, col + 1, DIR_UP_LEFT);
				addNewOpenList(row, col, row - 1, col + 1, DIR_UP_RIGHT);
				addNewOpenList(row, col, row + 1, col - 1, DIR_DOWN_LEFT);
				addNewOpenList(row, col, row - 1, col - 1, DIR_DOWN_RIGHT);
			}

			// �ҵ���ֵ��С�ĵ㣬������һ���㷨
			int cost = 0x7fffffff;
			for (int i = 0; i < mMapRowCount; i++) {
				for (int j = 0; j < mMapColumnCount; j++) {
					if (mOpenList[i][j][ISEXIST] == EXIST) {
						if (cost > getCost(i, j)) {
							cost = getCost(i, j);
							row = i;
							col = j;
						}
					}
				}
			}
		}
		// �㷨����
		Log.e(TAG, "Too big map, RowCount=" + mMapRowCount + ",ColCount" + mMapColumnCount);
		isFound = false;
		return;
	}

	// ���һ���µĽڵ�
	private void addNewOpenList(int sRow, int sCol, int eRow, int eCol, int dir) {
		if (isCanPass(eRow, eCol)) {
			if (mOpenList[eRow][eCol][ISEXIST] == EXIST) {
				if (mOpenList[sRow][sCol][EXPENSE] + getMapExpense(eRow, eCol, dir) < mOpenList[eRow][eCol][EXPENSE]) {
					setFatherDir(eRow, eCol, dir);
					setCost(eRow, eCol, sRow, sCol, dir);
				}
			} else {
				addOpenList(eRow, eCol);
				setFatherDir(eRow, eCol, dir);
				setCost(eRow, eCol, sRow, sCol, dir);
			}
		}
	}

	// ��������ֵ
	private void setCost(int sRow, int sCol, int eRow, int eCol, int dir) {
		mOpenList[sRow][sCol][EXPENSE] = mOpenList[eRow][eCol][EXPENSE]
				+ getMapExpense(sRow, sCol, dir);
		mOpenList[sRow][sCol][DISTANCE] = getDistance(sRow, sCol, eRow, eCol);
		mOpenList[sRow][sCol][COST] = mOpenList[sRow][sCol][EXPENSE]
				+ mOpenList[sRow][sCol][DISTANCE];
	}

	// ���ø��ڵ㷽��
	private void setFatherDir(int iRow, int iCol, int iDir) {
		mOpenList[iRow][iCol][FATHER_DIR] = iDir;
	}

	//������
	protected boolean checkPass(int iRow, int iCol){
		if (mMap[iRow][iCol] != 325) {
			return false;
		}
		return true;
	}
	
	//private String sTmpReason;
	// �ж�һ�����Ƿ����ͨ��
	private boolean isCanPass(int iRow, int iCol) {
		// �����߽�
		if (iRow < 0 || iRow >= mMapRowCount || iCol < 0 || iCol >= mMapColumnCount) {
			//sTmpReason = "OverBounds, iRow=" + iRow + ",nCol=" + iCol 
			//		+ ",mMapRowCount=" + mMapRowCount +",mMapColumnCount=" + mMapColumnCount;
			return false;
		}
		
		// ��ͼ��ͨ
		if (!checkPass(iRow, iCol)) {
			//sTmpReason = "checkPass";
			return false;
		}
		// �ڹر��б���
		if (isInCloseList(iRow, iCol)) {
			//sTmpReason = "inCloseList, iRow=" + iRow + ",nCol=" + iCol;
			return false;
		}
		//sTmpReason = "";
		return true;
	}

	// �Ƴ����б��һ��Ԫ��
	private void removeOpenList(int iRow, int iCol) {
		if (mOpenList[iRow][iCol][ISEXIST] == EXIST) {
			mOpenList[iRow][iCol][ISEXIST] = NOT_EXIST;
			mOpenListLength--;
		}
	}

	// �ж�һ���Ƿ��ڹر��б���
	private boolean isInCloseList(int iRow, int iCol) {
		return mCloseList[iRow][iCol];
	}

	// ��ӹر��б�
	private void addCloseList(int iRow, int iCol) {
		mCloseList[iRow][iCol] = true;
	}

	// ��Ӵ��б�
	private void addOpenList(int iRow, int iCol) {
		if (mOpenList[iRow][iCol][ISEXIST] == NOT_EXIST) {
			mOpenList[iRow][iCol][ISEXIST] = EXIST;
			mOpenListLength++;
		}
	}

	// ��ʼ���ر��б�
	private void initCloseList() {
		mCloseList = new boolean[mMapRowCount][mMapColumnCount];
		for (int row = 0; row < mMapRowCount; row++) {
			for (int col = 0; col < mMapColumnCount; col++) {
				mCloseList[row][col] = false;
			}
		}
	}

	// ��ʼ�����б�
	private void initOpenList(int endRow, int endCol) {
		mOpenList = new int[mMapRowCount][mMapColumnCount][5];
		for (int row = 0; row < mMapRowCount; row++) {
			for (int col = 0; col < mMapColumnCount; col++) {
				mOpenList[row][col][ISEXIST] = NOT_EXIST;
				mOpenList[row][col][EXPENSE] = getMapExpense(row, col, DIR_NULL);
				mOpenList[row][col][DISTANCE] = getDistance(row, col, endRow, endCol);
				mOpenList[row][col][COST] = mOpenList[row][col][EXPENSE] + mOpenList[row][col][DISTANCE];
				mOpenList[row][col][FATHER_DIR] = DIR_NULL;
			}
		}
		mOpenListLength = 0;
	}

	public String getDirString(int dir){
		switch (dir) {
		case DIR_NULL: 					return "N";
		case DIR_DOWN: 				return "��";
		case DIR_UP:						return "��";
		case DIR_LEFT:					return "��";
		case DIR_RIGHT:				return "��";
		case DIR_UP_LEFT:				return "�I";
		case DIR_UP_RIGHT:			return "�J";
		case DIR_DOWN_LEFT:		return "�L";
		case DIR_DOWN_RIGHT:		return "�K";
		default:									return "Unknown";
		}
	}
	// ���Ѱ·���
	public PosInfo[] getResult(boolean includeStart) {
		PosInfo[] result;
		ArrayList<PosInfo> route;
		searchPath();
		if (!isFound) {
			return null;
		}
		route = new ArrayList<PosInfo>();
		// openList�Ǵ�Ŀ�������ʼ�㵹�Ƶġ�
		int iX = mEndRow;
		int iY = mEndCol;
		while ((iX != mStartRow || iY != mStartCol)) {
			route.add(new PosInfo(iX, iY, mOpenList[iX][iY][FATHER_DIR]));
			switch (mOpenList[iX][iY][FATHER_DIR]) {
			case DIR_DOWN:
				iY++;
				break;
			case DIR_UP:
				iY--;
				break;
			case DIR_LEFT:
				iX--;
				break;
			case DIR_RIGHT:
				iX++;
				break;
			case DIR_UP_LEFT:
				iX--;
				iY--;
				break;
			case DIR_UP_RIGHT:
				iX++;
				iY--;
				break;
			case DIR_DOWN_LEFT:
				iX--;
				iY++;
				break;
			case DIR_DOWN_RIGHT:
				iX++;
				iY++;
				break;
			}
		}
		if (includeStart) {
			route.add(new PosInfo(mStartRow, mStartCol, mOpenList[mStartRow][mStartCol][FATHER_DIR]));
		}
		int size = route.size();
		result = new PosInfo[size];
		for (int i = 0; i < size; i++) {
			result[i] = new PosInfo((PosInfo) route.get(i));
		}
		return result;
	}
}