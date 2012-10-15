package cn.shangcm.boxz.android.controlpanel;

import android.content.Context;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Rect;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import cn.shangcm.boxz.android.R;
import cn.shangcm.boxz.android.enums.ControlCommand;

/**
 * Accelerometer implement of the control panel
 * @version $Id$
 * @author shangcm
 */
public class AccelerometerControlPanel extends View implements ControlPanel, SensorEventListener {

	private static final String TAG = "AccelerometerControlPanel";

	private final int buttonWidth;

	private final int buttonSpace;

	private final int halfButtonWidth;

	private Context context;

	private ControlCommandListener commandListener;

	private SensorManager sensorManager;

	private ControlCommand lastMoveCommand;

	private Rect rectLeftArmUp;

	private Rect rectLeftArmDown;

	private Rect rectRightArmUp;

	private Rect rectRightArmDown;

	private Rect rectBothArmUp;

	private Rect rectBothArmDown;

	private Rect rectSpecialSkill;

	/**
	 * This creates a AccelerometerControlPanel
	 * @param context
	 * @param screenWidth
	 * @param screenHeight
	 */
	public AccelerometerControlPanel(Context context, int screenWidth, int screenHeight) {
		super(context);
		this.context = context;
		this.sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
		// this.screenHeight = screenHeight;
		this.buttonWidth = screenWidth / 8;
		this.buttonSpace = screenWidth / 40;
		this.halfButtonWidth = this.buttonWidth / 2;
		this.rectLeftArmUp = new Rect(this.buttonWidth, this.buttonWidth, this.buttonWidth * 2, this.buttonWidth * 2);
		this.rectLeftArmDown = new Rect(this.buttonWidth, this.buttonWidth * 2 + this.buttonSpace, this.buttonWidth * 2, this.buttonWidth * 3
				+ this.buttonSpace);
		this.rectRightArmUp = new Rect(this.buttonWidth * 6, this.buttonWidth, this.buttonWidth * 7, this.buttonWidth * 2);
		this.rectRightArmDown = new Rect(this.buttonWidth * 6, this.buttonWidth * 2 + this.buttonSpace, this.buttonWidth * 7, this.buttonWidth * 3
				+ this.buttonSpace);
		this.rectBothArmUp = new Rect(this.buttonWidth * 3, this.buttonWidth, this.buttonWidth * 5, this.buttonWidth * 2);
		this.rectBothArmDown = new Rect(this.buttonWidth * 3, this.buttonWidth * 2 + this.buttonSpace, this.buttonWidth * 5, this.buttonWidth * 3
				+ this.buttonSpace);
		this.rectSpecialSkill = new Rect(this.buttonWidth * 2 + this.halfButtonWidth, this.buttonWidth * 3 + this.halfButtonWidth, this.buttonWidth * 5
				+ this.halfButtonWidth, this.buttonWidth * 4 + this.halfButtonWidth);
	}

	/**
	 * {@inheritDoc}
	 * @see cn.shangcm.boxz.android.controlpanel.ControlPanel#setCommandListener(cn.shangcm.boxz.android.controlpanel.ControlCommandListener)
	 */
	public void setCommandListener(ControlCommandListener commandListener) {
		this.commandListener = commandListener;
	}

	/**
	 * {@inheritDoc}
	 * @see android.view.View#onDraw(android.graphics.Canvas)
	 */
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		Paint paint = new Paint();
		paint.setColor(Color.WHITE);
		// app name & version
		String ver = "1.0.1";
		try {
			ver = this.context.getPackageManager().getPackageInfo(this.context.getPackageName(), PackageManager.PERMISSION_GRANTED).versionName;
		} catch (NameNotFoundException ex) {
			// do nothing
		}
		canvas.drawText(this.getResources().getString(R.string.app_name) + " " + ver, 10, 10, paint);
		paint.setColor(Color.GREEN);
		paint.setStyle(Style.STROKE);
		// left arm up
		canvas.drawRect(this.rectLeftArmUp, paint);
		canvas.drawLine(this.rectLeftArmUp.left + this.buttonSpace, this.rectLeftArmUp.bottom - this.buttonSpace, this.rectLeftArmUp.left
				+ this.halfButtonWidth, this.rectLeftArmUp.top + this.buttonSpace, paint);
		canvas.drawLine(this.rectLeftArmUp.left + this.halfButtonWidth, this.rectLeftArmUp.top + this.buttonSpace, this.rectLeftArmUp.right - this.buttonSpace,
				this.rectLeftArmUp.bottom - this.buttonSpace, paint);
		// left arm down
		canvas.drawRect(this.rectLeftArmDown, paint);
		canvas.drawLine(this.rectLeftArmDown.left + this.buttonSpace, this.rectLeftArmDown.top + this.buttonSpace, this.rectLeftArmDown.left
				+ this.halfButtonWidth, this.rectLeftArmDown.bottom - this.buttonSpace, paint);
		canvas.drawLine(this.rectLeftArmDown.left + this.halfButtonWidth, this.rectLeftArmDown.bottom - this.buttonSpace, this.rectLeftArmDown.right
				- this.buttonSpace, this.rectLeftArmDown.top + this.buttonSpace, paint);
		// right arm up
		canvas.drawRect(this.rectRightArmUp, paint);
		canvas.drawLine(this.rectRightArmUp.left + this.buttonSpace, this.rectRightArmUp.bottom - this.buttonSpace, this.rectRightArmUp.left
				+ this.halfButtonWidth, this.rectRightArmUp.top + this.buttonSpace, paint);
		canvas.drawLine(this.rectRightArmUp.left + this.halfButtonWidth, this.rectRightArmUp.top + this.buttonSpace, this.rectRightArmUp.right
				- this.buttonSpace, this.rectRightArmUp.bottom - this.buttonSpace, paint);
		// right arm down
		canvas.drawRect(this.rectRightArmDown, paint);
		canvas.drawLine(this.rectRightArmDown.left + this.buttonSpace, this.rectRightArmDown.top + this.buttonSpace, this.rectRightArmDown.left
				+ this.halfButtonWidth, this.rectRightArmDown.bottom - this.buttonSpace, paint);
		canvas.drawLine(this.rectRightArmDown.left + this.halfButtonWidth, this.rectRightArmDown.bottom - this.buttonSpace, this.rectRightArmDown.right
				- this.buttonSpace, this.rectRightArmDown.top + this.buttonSpace, paint);
		// both arm up
		canvas.drawRect(this.rectBothArmUp, paint);
		canvas.drawLine(this.rectBothArmUp.left + this.buttonSpace, this.rectBothArmUp.bottom - this.buttonSpace, this.rectBothArmUp.left
				+ this.halfButtonWidth, this.rectBothArmUp.top + this.buttonSpace, paint);
		canvas.drawLine(this.rectBothArmUp.left + this.halfButtonWidth, this.rectBothArmUp.top + this.buttonSpace, this.rectBothArmUp.right - this.buttonWidth
				- this.buttonSpace, this.rectBothArmUp.bottom - this.buttonSpace, paint);
		canvas.drawLine(this.rectBothArmUp.left + this.buttonWidth + this.buttonSpace, this.rectBothArmUp.bottom - this.buttonSpace, this.rectBothArmUp.left
				+ this.buttonWidth + this.halfButtonWidth, this.rectBothArmUp.top + this.buttonSpace, paint);
		canvas.drawLine(this.rectBothArmUp.left + this.buttonWidth + this.halfButtonWidth, this.rectBothArmUp.top + this.buttonSpace, this.rectBothArmUp.right
				- this.buttonSpace, this.rectBothArmUp.bottom - this.buttonSpace, paint);
		// both arm down
		canvas.drawRect(this.rectBothArmDown, paint);
		canvas.drawLine(this.rectBothArmDown.left + this.buttonSpace, this.rectBothArmDown.top + this.buttonSpace, this.rectBothArmDown.left
				+ this.halfButtonWidth, this.rectBothArmDown.bottom - this.buttonSpace, paint);
		canvas.drawLine(this.rectBothArmDown.left + this.halfButtonWidth, this.rectBothArmDown.bottom - this.buttonSpace, this.rectBothArmDown.right
				- this.buttonWidth - this.buttonSpace, this.rectBothArmDown.top + this.buttonSpace, paint);
		canvas.drawLine(this.rectBothArmDown.left + this.buttonWidth + this.buttonSpace, this.rectBothArmDown.top + this.buttonSpace, this.rectBothArmDown.left
				+ this.buttonWidth + this.halfButtonWidth, this.rectBothArmDown.bottom - this.buttonSpace, paint);
		canvas.drawLine(this.rectBothArmDown.left + this.buttonWidth + this.halfButtonWidth, this.rectBothArmDown.bottom - this.buttonSpace,
				this.rectBothArmDown.right - this.buttonSpace, this.rectBothArmDown.top + this.buttonSpace, paint);
		// special skill
		canvas.drawRect(this.rectSpecialSkill, paint);
	}

	/**
	 * {@inheritDoc}
	 * @see android.view.View#onTouchEvent(android.view.MotionEvent)
	 */
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (this.commandListener != null) {
			ControlCommand command = this.getActionCommand((int) event.getX(), (int) event.getY());
			Log.v(TAG, "action command=" + command);
			if (command != null) {
				this.commandListener.onCommand(command);
			}
		}
		return super.onTouchEvent(event);
	}

	/**
	 * {@inheritDoc}
	 * @see android.hardware.SensorEventListener#onAccuracyChanged(android.hardware.Sensor, int)
	 */
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// do nothing
	}

	/**
	 * {@inheritDoc}
	 * @see android.hardware.SensorEventListener#onSensorChanged(android.hardware.SensorEvent)
	 */
	public void onSensorChanged(SensorEvent event) {
		if ((event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) && (this.commandListener != null)) {
			ControlCommand command = this.getMoveCommand((int) (event.values[0] * 10), (int) (event.values[1] * 10));
			Log.v(TAG, "lastMoveCommand=" + this.lastMoveCommand + ", new move command=" + command);
			if ((command != null) && ((this.lastMoveCommand == null) || (this.lastMoveCommand != command))) {
				Log.d(TAG, "move command " + command.name() + " triggered.");
				this.lastMoveCommand = command;
				this.commandListener.onCommand(command);
			}
		}
	}

	/**
	 * {@inheritDoc}
	 * @see cn.shangcm.boxz.android.controlpanel.ControlPanel#start()
	 */
	public void start() {
		this.sensorManager.registerListener(this, this.sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_GAME);
		this.invalidate();
		Log.i(TAG, "AccelerometerControlPanel started.");
	}

	/**
	 * {@inheritDoc}
	 * @see cn.shangcm.boxz.android.controlpanel.ControlPanel#stop()
	 */
	public void stop() {
		this.sensorManager.unregisterListener(this);
		Log.i(TAG, "AccelerometerControlPanel stopped.");
	}

	/**
	 * to get the move command from the accelerometer sensor
	 * @param x
	 * @param y
	 * @return the control command
	 */
	private ControlCommand getMoveCommand(int x, int y) {
		// Log.v(TAG, "move position: x=" + x + ", y=" + y);
		if ((Math.abs(x) <= 15) && (Math.abs(y) <= 15)) {
			// stop
			return ControlCommand.STOP;
		}
		if ((x < -15) && (Math.abs(y) <= 15)) {
			// forward
			return ControlCommand.MOVE_FORWARD;
		}
		if ((x > 15) && (Math.abs(y) <= 15)) {
			// backward
			return ControlCommand.MOVE_BACKWARD;
		}
		if ((Math.abs(x) <= 15) && (y < -15)) {
			// turn left
			return ControlCommand.TURN_LEFT;
		}
		if ((Math.abs(x) <= 15) && (y > 15)) {
			// turn left
			return ControlCommand.TURN_RIGHT;
		}
		return null;
	}

	/**
	 * to get the action command from touch
	 * @param x
	 * @param y
	 * @return the action command
	 */
	private ControlCommand getActionCommand(int x, int y) {
		Log.v(TAG, "action position: x=" + x + ", y=" + y);
		if (this.rectLeftArmUp.contains(x, y)) {
			return ControlCommand.LEFT_ARM_UP;
		}
		if (this.rectLeftArmDown.contains(x, y)) {
			return ControlCommand.LEFT_ARM_DOWN;
		}
		if (this.rectRightArmUp.contains(x, y)) {
			return ControlCommand.RIGHT_ARM_UP;
		}
		if (this.rectRightArmDown.contains(x, y)) {
			return ControlCommand.RIGHT_ARM_DOWN;
		}
		if (this.rectBothArmUp.contains(x, y)) {
			return ControlCommand.BOTH_ARM_UP;
		}
		if (this.rectBothArmDown.contains(x, y)) {
			return ControlCommand.BOTH_ARM_DOWN;
		}
		if (this.rectRightArmUp.contains(x, y)) {
			return ControlCommand.RIGHT_ARM_UP;
		}
		if (this.rectSpecialSkill.contains(x, y)) {
			return ControlCommand.SPECIAL_SKILL;
		}
		return null;
	}

}
