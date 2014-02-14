package com.fishjam.storehelper;

import android.app.Activity;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;
import android.view.WindowManager;

public class PositionActivity extends Activity {

	private int window_width, window_height;// �ؼ����
	private PositionView positionView;// �Զ���ؼ�
	private int state_height;// ״̬���ĸ߶�

	private ViewTreeObserver viewTreeObserver;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		// setContentView(R.layout.main);
		/** ��ȡ��Ҋ����߶� **/
		WindowManager manager = getWindowManager();
		window_width = manager.getDefaultDisplay().getWidth();
		window_height = manager.getDefaultDisplay().getHeight();

		positionView = new PositionView(this);
		// positionView.setScaleType(ImageView.ScaleType.CENTER);
		positionView.setFloor(StoreInformation.Instance(this).mCurPositionInfo.iFloor);
		setContentView(positionView);

		// dragImageView = (DragImageView) findViewById(R.id.div_main);
		// Bitmap bmp = BitmapUtil.ReadBitmapById(this, R.drawable.huoying,
		// window_width, window_height);
		// ����ͼƬ

		/** ����״̬���߶� **/
		viewTreeObserver = positionView.getViewTreeObserver();
		viewTreeObserver.addOnGlobalLayoutListener(new OnGlobalLayoutListener() {

			@Override
			public void onGlobalLayout() {
				if (state_height == 0) {
					// ��ȡ״�����߶�
					Rect frame = new Rect();
					getWindow().getDecorView()
							.getWindowVisibleDisplayFrame(frame);
					state_height = frame.top;
					Log.i("onGlobalLayout", "new window_h="
							+ (window_height - state_height)
							+ ",Screen_W=" + window_width);
					positionView.setScreen_H(window_height
							- state_height);
					positionView.setScreen_W(window_width);
				}
			}
		});
	}

}
