package com.fishjam.util;

import android.util.Log;
import android.view.View;
import android.view.ViewTreeObserver;

/***************************************************************************************************************************************
 * View�仯������
 * 
 ***************************************************************************************************************************************/

public class DefaultViewTreeObserverHanlder implements
		ViewTreeObserver.OnGlobalFocusChangeListener,	//���ڼ�������ı仯 
		ViewTreeObserver.OnGlobalLayoutListener,				//���ڼ�������֮��ı仯������ĳ���ؼ�������
		ViewTreeObserver.OnPreDrawListener,					//��������Ļ�ϻ� View ֮ǰ��Ҫ��ʲô����Ĺ��������� setHint ��?
		ViewTreeObserver.OnScrollChangedListener,
		ViewTreeObserver.OnTouchModeChangeListener		//���ڼ��� Touch �ͷ� Touch ģʽ��ת��  
{	
	private final static String TAG = DefaultViewTreeObserverHanlder.class.getSimpleName();

	private View mView;

	public DefaultViewTreeObserverHanlder(View view) {
		mView = view;
	}

	public void setupListener() {
		final ViewTreeObserver viewTreeObserver = mView.getViewTreeObserver();
		if (viewTreeObserver != null) {
			viewTreeObserver.addOnGlobalLayoutListener(this);
			viewTreeObserver.addOnGlobalFocusChangeListener(this);
			viewTreeObserver.addOnPreDrawListener(this);
			viewTreeObserver.addOnScrollChangedListener(this);
			viewTreeObserver.addOnTouchModeChangeListener(this);
		}
	}

	public void tearDownListener() {
		final ViewTreeObserver viewTreeObserver = mView.getViewTreeObserver();
		if (viewTreeObserver != null) {
			viewTreeObserver.removeGlobalOnLayoutListener(this);
			viewTreeObserver.removeOnGlobalFocusChangeListener(this);
			viewTreeObserver.removeOnPreDrawListener(this);
			viewTreeObserver.removeOnScrollChangedListener(this);
			viewTreeObserver.removeOnTouchModeChangeListener(this);
		}
	}

	@Override
	public void onGlobalFocusChanged(View oldFocus, View newFocus) {
		Log.i(TAG, "onGlobalFocusChanged, from " + oldFocus.toString() + " to " + newFocus.toString());
	}

	@Override
	public void onTouchModeChanged(boolean isInTouchMode) {
		Log.i(TAG, "onTouchModeChanged, isInTouchMode=" + isInTouchMode);

	}

	@Override
	public void onScrollChanged() {
		Log.i(TAG, "onScrollChanged");

	}

	@Override
	public boolean onPreDraw() {
		Log.i(TAG, "onPreDraw");
		
		return true;		//true ��ʾ�������ƣ� false ��ʾȡ��(����ɽ�����ʾ������)
	}

	@Override
	public void onGlobalLayout() {
		Log.i(TAG, "onGlobalLayout");
	}
}
