package cn.shangcm.boxz.android;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
import cn.shangcm.boxz.android.controlpanel.AccelerometerControlPanel;
import cn.shangcm.boxz.android.controlpanel.ControlCommandListener;
import cn.shangcm.boxz.android.controlpanel.ControlPanel;
import cn.shangcm.boxz.android.enums.ControlCommand;

/**
 * main activity
 * @version $Id$
 * @author shangcm
 */
public class MainActivity extends Activity {

	private static final String TAG = "Main";

	private static final String DEVICE_UUID = "00001101-0000-1000-8000-00805F9B34FB";

	private static final int ABOUT_DIALOG = 2001;

	private static final int CONNECT_ERROR_DIALOG = 2002;

	private static final int REQ_SETTINGS = 1001;

	private static final int SHOW_MAIN = 3001;

	protected Map<ControlCommand, Integer> commandMapping;

	protected BluetoothDevice device;

	protected ProgressDialog progressDialog;

	protected BluetoothSocket socket;

	protected InputStream is;

	protected OutputStream os;

	private ControlPanel controlPanel;

	protected Handler uiHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
				case SHOW_MAIN:
					MainActivity.this.progressDialog.dismiss();
					if (MainActivity.this.os == null) {
						MainActivity.this.showDialog(CONNECT_ERROR_DIALOG);
					}
					break;
			}
		}

	};

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		// initialize UI
		this.initializeUI();
		// load command mapping
		this.loadCommandMapping();
		// get selected device
		this.device = getIntent().getParcelableExtra("device");
		// connect to device
		this.progressDialog = ProgressDialog.show(this, "connecting", "connect to '" + this.device.getName() + "'...");
		(new Thread() {

			/**
			 * {@inheritDoc}
			 * @see java.lang.Thread#run()
			 */
			@Override
			public void run() {
				UUID uuid = UUID.fromString(DEVICE_UUID);
				try {
					MainActivity.this.socket = MainActivity.this.device.createRfcommSocketToServiceRecord(uuid);
					MainActivity.this.socket.connect();
					MainActivity.this.is = MainActivity.this.socket.getInputStream();
					MainActivity.this.os = MainActivity.this.socket.getOutputStream();
					MainActivity.this.uiHandler.sendEmptyMessage(SHOW_MAIN);
				} catch (IOException ex) {
					MainActivity.this.progressDialog.dismiss();
					Log.e(TAG, "connect to device error: " + ex.getMessage(), ex);
					MainActivity.this.showDialog(CONNECT_ERROR_DIALOG);
				}
			}

		}).start();
	}

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onCreateOptionsMenu(android.view.Menu)
	 */
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		this.getMenuInflater().inflate(R.menu.options_menu, menu);
		return true;
	}

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onMenuItemSelected(int, android.view.MenuItem)
	 */
	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		switch (item.getItemId()) {
			case R.id.setting_menu:
				startActivityForResult(new Intent(this, SettingActivity.class), REQ_SETTINGS);
				break;
			case R.id.about_menu:
				this.showDialog(ABOUT_DIALOG);
				break;
		}
		return super.onMenuItemSelected(featureId, item);
	}

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onCreateDialog(int)
	 */
	@Override
	public Dialog onCreateDialog(int id) {
		switch (id) {
			case ABOUT_DIALOG:
				// about
				return new AlertDialog.Builder(MainActivity.this).setTitle(R.string.about_title).setMessage(R.string.about_content)
						.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {

							public void onClick(DialogInterface dialog, int whichButton) {
								// do nothing
							}

						}).create();
			case CONNECT_ERROR_DIALOG:
				return new AlertDialog.Builder(MainActivity.this).setTitle(R.string.error_title)
						.setMessage(MainActivity.this.getResources().getString(R.string.error_connect_device, MainActivity.this.device.getName()))
						.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {

							public void onClick(DialogInterface dialog, int whichButton) {
								MainActivity.this.finish();
							}

						}).create();
		}
		return super.onCreateDialog(id);
	}

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onActivityResult(int, int, android.content.Intent)
	 */
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == REQ_SETTINGS) {
			this.loadCommandMapping();
		}
	}

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onStop()
	 */
	@Override
	protected void onStop() {
		this.controlPanel.stop();
		if (this.is != null) {
			try {
				this.is.close();
			} catch (IOException ex) {
				Log.e(TAG, "close inputStream error: " + ex.getMessage(), ex);
			}
		}
		if (this.os != null) {
			try {
				this.os.close();
			} catch (IOException ex) {
				Log.e(TAG, "close outputStream error: " + ex.getMessage(), ex);
			}
		}
		if (this.socket != null) {
			try {
				this.socket.close();
			} catch (IOException ex) {
				Log.e(TAG, "close socket error: " + ex.getMessage(), ex);
			}
		}
		super.onStop();
	}

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onPause()
	 */
	@Override
	protected void onPause() {
		this.controlPanel.stop();
		super.onPause();
	}

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onResume()
	 */
	@Override
	protected void onResume() {
		this.controlPanel.start();
		super.onResume();
	}

	/**
	 * to load control command mapping from Shared Preference
	 */
	private void loadCommandMapping() {
		this.commandMapping = new HashMap<ControlCommand, Integer>();
		SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(this);
		for (ControlCommand c : ControlCommand.values()) {
			this.commandMapping.put(c, (int) settings.getString(c.name(), String.valueOf(c.getDefaultValue())).charAt(0));
		}
	}

	/**
	 * to send the control command to the device
	 * @param command the control command
	 */
	protected void sendCommand(ControlCommand command) {
		if (command == null) {
			return;
		}
		if (this.os == null) {
			return;
		}
		int value = MainActivity.this.commandMapping.get(command);
		Log.i(TAG, "sending '" + ((char) value) + "' to device...");
		try {
			this.os.write(value);
		} catch (IOException ex) {
			Log.e(TAG, "send move command error: " + ex.getMessage(), ex);
		}
	}

	/**
	 * to initialize the UI
	 */
	private void initializeUI() {
		int screenWidth = this.getWindowManager().getDefaultDisplay().getWidth();
		int screenHeight = this.getWindowManager().getDefaultDisplay().getHeight();
		LinearLayout layout = (LinearLayout) findViewById(R.id.main_layout);
		this.controlPanel = new AccelerometerControlPanel(this, screenWidth, screenHeight);
		layout.addView((View) this.controlPanel);
		this.controlPanel.setCommandListener(new ControlCommandListener() {

			public void onCommand(ControlCommand command) {
				Log.i(TAG, "sending control command '" + command.name() + "' to device ...");
				sendCommand(command);
			}

		});
	}

}