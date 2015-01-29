/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.hmsoft.bluetooth.le;

import com.example.bluetooth.le.R;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;


/**
 * For a given BLE device, this Activity provides the user interface to connect, display data,
 * and display GATT services and characteristics supported by the device.  The Activity
 * communicates with {@code BluetoothLeService}, which in turn interacts with the
 * Bluetooth LE API.
 */
public class DeviceControlActivity extends Activity {
    private final static String TAG = DeviceControlActivity.class.getSimpleName();

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    private TextView mDataField;
    private String mDeviceName;
    private String mDeviceAddress;
    private BluetoothLeService mBluetoothLeService;
    private boolean mConnected = false;
    
    EditText edtSend;
	ScrollView svResult;
	Button btnSend;
	
	Button btnSendw;
	Button btnSenda;
	Button btnSends;
	Button btnSendd;
	Button btnSendno;
	Button btnSendq;
	Button btnSende;
	Button btnSendz;
	Button btnSendx;	

    // Code to manage Service lifecycle.
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            
            Log.e(TAG, "mBluetoothLeService is okay");
            // Automatically connects to the device upon successful start-up initialization.
            //mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {  //连接成功
            	Log.e(TAG, "Only gatt, just wait");
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) { //断开连接
                mConnected = false;
                invalidateOptionsMenu();
                btnSend.setEnabled(false);
                clearUI();
            }else if(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) //可以开始干活了
            {
            	mConnected = true;
            	mDataField.setText("");
            	ShowDialog();
            	btnSend.setEnabled(true);
            	Log.e(TAG, "In what we need");
            	invalidateOptionsMenu();
            }else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) { //收到数据
            	Log.e(TAG, "RECV DATA");
            	String data = intent.getStringExtra(BluetoothLeService.EXTRA_DATA);
            	if (data != null) {
                	if (mDataField.length() > 500)
                		mDataField.setText("");
                    mDataField.append(data); 
                    svResult.post(new Runnable() {
            			public void run() {
            				svResult.fullScroll(ScrollView.FOCUS_DOWN);
            			}
            		});
                }
            }
        }
    };

    private void clearUI() {
        mDataField.setText(R.string.no_data);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {                                        //初始化
        super.onCreate(savedInstanceState);
        setContentView(R.layout.gatt_services_characteristics);

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        // Sets up UI references.
        mDataField = (TextView) findViewById(R.id.data_value);
        edtSend = (EditText) this.findViewById(R.id.edtSend);
        edtSend.setText("{\"AT\":{\"K1\":\"w\"}}");
        svResult = (ScrollView) this.findViewById(R.id.svResult);
        
        btnSend = (Button) this.findViewById(R.id.btnSend);
		btnSend.setOnClickListener(new ClickEvent());
		btnSend.setEnabled(false);
		
        btnSendw = (Button) this.findViewById(R.id.btnSendw);
		btnSendw.setOnClickListener(new ClickEvent());		
        btnSenda = (Button) this.findViewById(R.id.btnSenda);
		btnSenda.setOnClickListener(new ClickEvent());	
        btnSends = (Button) this.findViewById(R.id.btnSends);
		btnSends.setOnClickListener(new ClickEvent());	
        btnSendd = (Button) this.findViewById(R.id.btnSendd);
		btnSendd.setOnClickListener(new ClickEvent());	
        btnSendno = (Button) this.findViewById(R.id.btnSendno);
		btnSendno.setOnClickListener(new ClickEvent());		
        btnSendq = (Button) this.findViewById(R.id.btnSendq);
		btnSendq.setOnClickListener(new ClickEvent());	
        btnSende = (Button) this.findViewById(R.id.btnSende);
		btnSende.setOnClickListener(new ClickEvent());	
        btnSendz = (Button) this.findViewById(R.id.btnSendz);
		btnSendz.setOnClickListener(new ClickEvent());	
        btnSendx = (Button) this.findViewById(R.id.btnSendx);
		btnSendx.setOnClickListener(new ClickEvent());			

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        Log.d(TAG, "Try to bindService=" + bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE));
        
        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
    }

    @Override
    protected void onResume() {
        super.onResume();
        /*registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
        if (mBluetoothLeService != null) {
            final boolean result = mBluetoothLeService.connect(mDeviceAddress);
            Log.d(TAG, "Connect request result=" + result);
        }*/
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mGattUpdateReceiver);
        unbindService(mServiceConnection);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        //this.unregisterReceiver(mGattUpdateReceiver);
        //unbindService(mServiceConnection);
        if(mBluetoothLeService != null)
        {
        	mBluetoothLeService.close();
        	mBluetoothLeService = null;
        }
        Log.d(TAG, "We are in destroy");
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.gatt_services, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {                              //点击按钮
        switch(item.getItemId()) {
            case R.id.menu_connect:
                mBluetoothLeService.connect(mDeviceAddress);
                return true;
            case R.id.menu_disconnect:
                mBluetoothLeService.disconnect();
                return true;
            case android.R.id.home:
            	if(mConnected)
            	{
            		mBluetoothLeService.disconnect();
            		mConnected = false;
            	}
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }
    
    private void ShowDialog()
    {
    	Toast.makeText(this, "连接成功，现在可以正常通信！", Toast.LENGTH_SHORT).show();
    }

 // 按钮事件
	class ClickEvent implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			if (v == btnSend) {
				if(!mConnected) return;
				
				if (edtSend.length() < 1) {
					Toast.makeText(DeviceControlActivity.this, "请输入要发送的内容", Toast.LENGTH_SHORT).show();
					return;
				}
				mBluetoothLeService.WriteValue(edtSend.getText().toString());
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}
			
            //操作发送向前信息w
			if (v == btnSendw) {
				if(!mConnected) return;
				mBluetoothLeService.WriteValue("{\"AT\":{\"K1\":\"w\"}}");
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}			
            //操作发送向左信息a
			if (v == btnSenda) {
				if(!mConnected) return;
				mBluetoothLeService.WriteValue("{\"AT\":{\"K1\":\"a\"}}");
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}
            //操作发送向后信息s
			if (v == btnSends) {
				if(!mConnected) return;
				mBluetoothLeService.WriteValue("{\"AT\":{\"K1\":\"s\"}}");
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}
            //操作发送向右信息d
			if (v == btnSendd) {
				if(!mConnected) return;
				mBluetoothLeService.WriteValue("{\"AT\":{\"K1\":\"d\"}}");
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}			
            //操作发送重置信息空格
			if (v == btnSendno) {
				if(!mConnected) return;
				mBluetoothLeService.WriteValue("{\"AT\":{\"K1\":\" \"}}");
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}			
            //操作发送左前信息q
			if (v == btnSendq) {
				if(!mConnected) return;
				mBluetoothLeService.WriteValue("{\"AT\":{\"K1\":\"q\"}}");
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}
            //操作发送右前信息e
			if (v == btnSende) {
				if(!mConnected) return;
				mBluetoothLeService.WriteValue("{\"AT\":{\"K1\":\"e\"}}");
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}			
            //操作发送左后信息z
			if (v == btnSendz) {
				if(!mConnected) return;
				mBluetoothLeService.WriteValue("{\"AT\":{\"K1\":\"z\"}}");
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}
            //操作发送右后信息x
			if (v == btnSendx) {
				if(!mConnected) return;
				mBluetoothLeService.WriteValue("{\"AT\":{\"K1\":\"x\"}}");
				
				InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				if(imm.isActive())
					imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
				//todo Send data
			}			
		}

	}
	
    private static IntentFilter makeGattUpdateIntentFilter() {                        //注册接收的事件
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        intentFilter.addAction(BluetoothDevice.ACTION_UUID);
        return intentFilter;
    }
}
