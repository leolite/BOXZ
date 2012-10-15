package cn.shangcm.boxz.android;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;

/**
 * device select activity
 * @version $Id$
 * @author shangcm
 */
public class DeviceSelectionActivity extends Activity {

	private static final String TAG = "DeviceSelector";

	private static final int SHOW_DEVICE_LIST = 1001;

	private static final int GOTO_MAIN = 1002;

	private BroadcastReceiver broadcastReceiver;

	protected ProgressDialog progressDialog;

	private ListView deviceListView;

	protected List<BluetoothDevice> deviceList;

	protected BluetoothDevice device;

	protected Handler uiHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			if (msg.what == SHOW_DEVICE_LIST) {
				if ((DeviceSelectionActivity.this.progressDialog != null) && (DeviceSelectionActivity.this.progressDialog.isShowing())) {
					DeviceSelectionActivity.this.progressDialog.dismiss();
				}
				showDeviceList();
			} else if (msg.what == GOTO_MAIN) {
				Log.i(TAG, "going to MainActivity ...");
				Intent intent = new Intent(DeviceSelectionActivity.this, MainActivity.class);
				intent.putExtra("device", DeviceSelectionActivity.this.device);
				startActivity(intent);
				DeviceSelectionActivity.this.finish();
			}
			super.handleMessage(msg);
		}

	};

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.device_selector);
		this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		this.deviceListView = (ListView) findViewById(R.id.deviceList);
		BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
		if (!adapter.isEnabled()) {
			Log.i(TAG, "opening the Bluetooth adapter ...");
			this.progressDialog = ProgressDialog.show(this, "opening", "open Bluetooth Adapter...");
			adapter.enable();
			this.broadcastReceiver = new BroadcastReceiver() {

				@Override
				public void onReceive(Context context, Intent intent) {
					if (intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR) == BluetoothAdapter.STATE_ON) {
						// bluetooth is on now
						Log.i(TAG, "the Bluetooth Adapter is on.");
						DeviceSelectionActivity.this.uiHandler.sendEmptyMessage(SHOW_DEVICE_LIST);
					}
				}

			};
			IntentFilter filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
			registerReceiver(this.broadcastReceiver, filter);
		} else {
			this.uiHandler.sendEmptyMessage(SHOW_DEVICE_LIST);
		}
	}

	protected void showDeviceList() {
		Log.d(TAG, "getting devices list ...");
		BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
		Set<BluetoothDevice> devices = adapter.getBondedDevices();
		if ((devices != null) && (!devices.isEmpty())) {
			Log.i(TAG, "devices list (size=" + devices.size() + "):");
			this.deviceList = new ArrayList<BluetoothDevice>(devices.size());
			ArrayAdapter<String> ad = new ArrayAdapter<String>(this, R.layout.device_list);
			for (BluetoothDevice d : devices) {
				ad.add(d.getName());
				this.deviceList.add(d);
				Log.i(TAG, "device: " + d.getName());
			}
			this.deviceListView.setAdapter(ad);
			this.deviceListView.setOnItemClickListener(new OnItemClickListener() {

				public void onItemClick(AdapterView<?> adv, View view, int position, long id) {
					DeviceSelectionActivity.this.device = DeviceSelectionActivity.this.deviceList.get(position);
					DeviceSelectionActivity.this.uiHandler.sendEmptyMessage(GOTO_MAIN);
				}

			});
		} else {
			Log.w(TAG, "no device found.");
		}
	}

	/**
	 * {@inheritDoc}
	 * @see android.app.Activity#onStop()
	 */
	@Override
	public void onStop() {
		if (this.broadcastReceiver != null) {
			try {
				unregisterReceiver(this.broadcastReceiver);
				Log.d(TAG, "the broadcast receiver unregistered.");
			} catch (Throwable ex) {
				Log.e(TAG, "unregister receiver error: " + ex.getMessage(), ex);
			}
		}
		super.onStop();
	}

}
