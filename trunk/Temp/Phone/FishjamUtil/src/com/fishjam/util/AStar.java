package com.fishjam.util;

import java.util.ArrayList;

import android.R.integer;

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
	
	class PosInfo {
		public PosInfo(int iX, int iY) {
			this.x = iX;
			this.y = iY;
		}

		public PosInfo(PosInfo posInfo) {
			this.x = posInfo.x;
			this.y = posInfo.y;
		}

		public int x;
		public int y;
	}

	private int[][] map; // ��ͼ����0��ʾ��ͨ����1��ʾ����ͨ��
	private int map_w; // ��ͼ���
	private int map_h; // ��ͼ�߶�
	private int start_x; // �������X
	private int start_y; // �������Y
	private int goal_x; // �յ�����X
	private int goal_y; // �յ�����Y
	private boolean mEnable8Way;		//�Ƿ�����8����Ѱ·��false��ʾ4����
	
	private boolean closeList[][]; // �ر��б�
	public int openList[][][]; // ���б�
	private int openListLength;

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

	private int astar_counter; // �㷨Ƕ�����
	private boolean isFound; // �Ƿ��ҵ�·��

	public AStar(int[][] mx, int sx, int sy, int gx, int gy, boolean bEnable8Way) {
		start_x = sx;
		start_y = sy;
		goal_x = gx;
		goal_y = gy;
		mEnable8Way = bEnable8Way;
		
		map = mx;
		map_w = mx.length;
		map_h = mx[0].length;
		
		astar_counter = 5000;
		initCloseList();
		initOpenList(goal_x, goal_y);
	}

	// �õ���ͼ����һ�������ֵ
	private int getMapExpense(int x, int y, int dir) {
		if (dir < 5) {
			return 10;
		} else {
			return 14;
		}
	}

	// �õ����������ֵ
	private int getDistance(int x, int y, int ex, int ey) {
		return 10 * (Math.abs(x - ex) + Math.abs(y - ey));
	}

	// �õ�����������Ӵ�ʱ��������ֵ
	private int getCost(int x, int y) {
		return openList[x][y][COST];
	}

	// ��ʼѰ·
	public void searchPath() {
		addOpenList(start_x, start_y);
		aStar(start_x, start_y);
	}

	// Ѱ·
	private void aStar(int x, int y) {
		// �����㷨���
		for (int t = 0; t < astar_counter; t++) {
			if (((x == goal_x) && (y == goal_y))) {
				isFound = true;
				return;
			} else if ((openListLength == 0)) {
				isFound = false;
				return;
			}

			removeOpenList(x, y);
			addCloseList(x, y);

			// �õ���Χ�ܹ����ߵĵ�
			addNewOpenList(x, y, x, y + 1, DIR_UP);
			addNewOpenList(x, y, x, y - 1, DIR_DOWN);
			addNewOpenList(x, y, x - 1, y, DIR_RIGHT);
			addNewOpenList(x, y, x + 1, y, DIR_LEFT);
			if (mEnable8Way) {
				addNewOpenList(x, y, x + 1, y + 1, DIR_UP_LEFT);
				addNewOpenList(x, y, x - 1, y + 1, DIR_UP_RIGHT);
				addNewOpenList(x, y, x + 1, y - 1, DIR_DOWN_LEFT);
				addNewOpenList(x, y, x - 1, y - 1, DIR_DOWN_RIGHT);
			}

			// �ҵ���ֵ��С�ĵ㣬������һ���㷨
			int cost = 0x7fffffff;
			for (int i = 0; i < map_w; i++) {
				for (int j = 0; j < map_h; j++) {
					if (openList[i][j][ISEXIST] == EXIST) {
						if (cost > getCost(i, j)) {
							cost = getCost(i, j);
							x = i;
							y = j;
						}
					}
				}
			}
		}
		// �㷨����
		isFound = false;
		return;
	}

	// ���һ���µĽڵ�
	private void addNewOpenList(int x, int y, int newX, int newY, int dir) {
		if (isCanPass(newX, newY)) {
			if (openList[newX][newY][ISEXIST] == EXIST) {
				if (openList[x][y][EXPENSE] + getMapExpense(newX, newY, dir) < openList[newX][newY][EXPENSE]) {
					setFatherDir(newX, newY, dir);
					setCost(newX, newY, x, y, dir);
				}
			} else {
				addOpenList(newX, newY);
				setFatherDir(newX, newY, dir);
				setCost(newX, newY, x, y, dir);
			}
		}
	}

	// ��������ֵ
	private void setCost(int x, int y, int ex, int ey, int dir) {
		openList[x][y][EXPENSE] = openList[ex][ey][EXPENSE]
				+ getMapExpense(x, y, dir);
		openList[x][y][DISTANCE] = getDistance(x, y, ex, ey);
		openList[x][y][COST] = openList[x][y][EXPENSE]
				+ openList[x][y][DISTANCE];
	}

	// ���ø��ڵ㷽��
	private void setFatherDir(int x, int y, int dir) {
		openList[x][y][FATHER_DIR] = dir;
	}

	// �ж�һ�����Ƿ����ͨ��
	private boolean isCanPass(int x, int y) {
		// �����߽�
		if (x < 0 || x >= map_w || y < 0 || y >= map_h) {
			return false;
		}
		// ��ͼ��ͨ
		if (map[x][y] != 0) {
			return false;
		}
		// �ڹر��б���
		if (isInCloseList(x, y)) {
			return false;
		}
		return true;
	}

	// �Ƴ����б��һ��Ԫ��
	private void removeOpenList(int x, int y) {
		if (openList[x][y][ISEXIST] == EXIST) {
			openList[x][y][ISEXIST] = NOT_EXIST;
			openListLength--;
		}
	}

	// �ж�һ���Ƿ��ڹر��б���
	private boolean isInCloseList(int x, int y) {
		return closeList[x][y];
	}

	// ��ӹر��б�
	private void addCloseList(int x, int y) {
		closeList[x][y] = true;
	}

	// ��Ӵ��б�
	private void addOpenList(int x, int y) {
		if (openList[x][y][ISEXIST] == NOT_EXIST) {
			openList[x][y][ISEXIST] = EXIST;
			openListLength++;
		}
	}

	// ��ʼ���ر��б�
	private void initCloseList() {
		closeList = new boolean[map_w][map_h];
		for (int i = 0; i < map_w; i++) {
			for (int j = 0; j < map_h; j++) {
				closeList[i][j] = false;
			}
		}
	}

	// ��ʼ�����б�
	private void initOpenList(int ex, int ey) {
		openList = new int[map_w][map_h][5];
		for (int i = 0; i < map_w; i++) {
			for (int j = 0; j < map_h; j++) {
				openList[i][j][ISEXIST] = NOT_EXIST;
				openList[i][j][EXPENSE] = getMapExpense(i, j, DIR_NULL);
				openList[i][j][DISTANCE] = getDistance(i, j, ex, ey);
				openList[i][j][COST] = openList[i][j][EXPENSE]
						+ openList[i][j][DISTANCE];
				openList[i][j][FATHER_DIR] = DIR_NULL;
			}
		}
		openListLength = 0;
	}

	// ���Ѱ·���
	public PosInfo[] getResult() {
		PosInfo[] result;
		ArrayList<PosInfo> route;
		searchPath();
		if (!isFound) {
			return null;
		}
		route = new ArrayList<PosInfo>();
		// openList�Ǵ�Ŀ�������ʼ�㵹�Ƶġ�
		int iX = goal_x;
		int iY = goal_y;
		while ((iX != start_x || iY != start_y)) {
			route.add(new PosInfo(iX, iY));
			switch (openList[iX][iY][FATHER_DIR]) {
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
		int size = route.size();
		result = new PosInfo[size];
		for (int i = 0; i < size; i++) {
			result[i] = new PosInfo((PosInfo) route.get(i));
		}
		return result;
	}
}