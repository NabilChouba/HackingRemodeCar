/*
 * Copyright (C) 2009 The Android Open Source Project
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

package com.example.android.BluetoothChat;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.LinearLayout;
import android.widget.ImageView;
import java.util.*;

import android.graphics.Color;
import android.graphics.Point;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;







import java.lang.Math;

/**
 * This is the main Activity that displays the current chat session.
 */
public class BluetoothChat extends Activity {
    // Debugging
    private static final String TAG = "BluetoothChat";
    private static final boolean D = true;

    // Message types sent from the BluetoothChatService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;

    // Key names received from the BluetoothChatService Handler
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";

    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;

    // Layout Views
    //private TextView mTitle;
    private ListView mConversationView ;
    private EditText mOutEditText;
    private Button mSendButton;
    private Button mSendButtonUp;
    private Button mSendButtonDown;
    private Button mSendButtonLeft;
    private Button mSendButtonRight;
    private Button mSendButtonDwL;        
    private Button mSendButtonDwR;
    private Button mSendButtonUpL;        
    private Button mSendButtonUpR;
    
    private Button mSendButtonTUpL;        
    private Button mSendButtonTUpR;
    private Button mSendButtonTUp;

    // Name of the connected device
    private String mConnectedDeviceName = null;
    // Array adapter for the conversation thread
    private ArrayAdapter<String> mConversationArrayAdapter;
    // String buffer for outgoing messages
    private StringBuffer mOutStringBuffer;
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
    // Member object for the chat services
    private BluetoothChatService mChatService = null;

    int pos_x_last,pos_y_last,pos_z_last,F_last;
    long mCpuTimeStamp_last;
    int t_pos_x[],t_pos_y[],t_pos_z[];
    int zi=0;
    int GoNextState = 0; 
    LinearLayout iLinearLayout;
    LinearLayout mLinearLayout;
    
    LinearLayout LayoutMain;
    TableLayout LayoutAllMove;
    TableRow    LayoutUp;
    TableRow    LayoutMidlle;
    TableRow    LayoutDown;
    TableRow    LayoutTrubo; 


    ImageView imgViewNumber1;
    ImageView imgViewNumber2;
    ImageView imgViewNumber3;
    ImageView imgViewNumber4;
    
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(D) Log.e(TAG, "+++ ON CREATE +++");

        // Set up the window layout
        //requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.main);
        //getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.custom_title);

        // Set up the custom title
        //mTitle = (TextView) findViewById(R.id.title_left_text);
        //mTitle.setText(R.string.app_name);
        //mTitle = (TextView) findViewById(R.id.title_right_text);

        
        
        // Get local Bluetooth adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		t_pos_x = new int [10];
		t_pos_y = new int [10];
		t_pos_z = new int [10];
        //NABIL
        for (int i = 0; i<10; i++) {
			t_pos_x[i] = 0;
			t_pos_y[i] = 0;
			t_pos_z[i] = 0;
		}


        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
     
        LayoutAllMove = (TableLayout)findViewById(R.id.tableLayoutRowAll);
        
        LayoutUp= (TableRow)findViewById(R.id.tableRowUP);
        LayoutMidlle= (TableRow)findViewById(R.id.tableRowMidle);
        LayoutDown= (TableRow)findViewById(R.id.tableRowDown);
        LayoutMain= (LinearLayout) findViewById(R.id.tableLayoutMain);
        LayoutTrubo = (TableRow) findViewById(R.id.tableRowTurbo); 

        
    }

    @Override
    public void onStart() {
        super.onStart();
        if(D) Log.e(TAG, "++ ON START ++");

        // If BT is not on, request that it be enabled.
        // setupChat() will then be called during onActivityResult
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        // Otherwise, setup the chat session
        } else {
            if (mChatService == null) setupChat();
        }
    }

    @Override
    public synchronized void onResume() {
        super.onResume();
        if(D) Log.e(TAG, "+ ON RESUME +");

        // Performing this check in onResume() covers the case in which BT was
        // not enabled during onStart(), so we were paused to enable it...
        // onResume() will be called when ACTION_REQUEST_ENABLE activity returns.
        if (mChatService != null) {
            // Only if the state is STATE_NONE, do we know that we haven't started already
            if (mChatService.getState() == BluetoothChatService.STATE_NONE) {
              // Start the Bluetooth chat services
              mChatService.start();
            }
        }
    }
    ImageView imgView1 ;
    ImageView imgView2 ;
    ImageView imgView3 ;
    ImageView imgView4 ;
    ImageView imgView5 ;
    ImageView imgView6 ;
    ImageView imgView7 ;
    Timer myTimer ;
    
    private void setupChat() {
        Log.d(TAG, "setupChat()");

        // Initialize the array adapter for the conversation thread
        mConversationArrayAdapter = new ArrayAdapter<String>(this, R.layout.message);
        mConversationView = (ListView) findViewById(R.id.in);
        mConversationView.setAdapter(mConversationArrayAdapter);
        
        mConversationArrayAdapter.add("hello");
        mConversationArrayAdapter.add("hello2");

        // Initialize the compose field with a listener for the return key
        mOutEditText = (EditText) findViewById(R.id.edit_text_out);
        mOutEditText.setOnEditorActionListener(mWriteListener);

        // Initialize the send button with a listener that for click events
        mSendButton = (Button) findViewById(R.id.button_send);
        mSendButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // Send a message using content of the edit text widget
                TextView view = (TextView) findViewById(R.id.edit_text_out);
                String message = view.getText().toString();
                sendMessage(message);
            }
        });

        
        mSendButtonDown = (Button) findViewById(R.id.buttonDown);        
        mSendButtonLeft = (Button) findViewById(R.id.buttonLeft);
        mSendButtonRight = (Button) findViewById(R.id.buttonRight);
        mSendButtonUp = (Button) findViewById(R.id.buttonUp);

        mSendButtonDwL = (Button) findViewById(R.id.buttonDWL);        
        mSendButtonDwR = (Button) findViewById(R.id.buttonDWR);
        mSendButtonUpL = (Button) findViewById(R.id.buttonUPL);        
        mSendButtonUpR = (Button) findViewById(R.id.buttonUPR);

        mSendButtonTUpL = (Button) findViewById(R.id.ButtonTUPL);        
        mSendButtonTUpR = (Button) findViewById(R.id.ButtonTUPR);
        mSendButtonTUp  = (Button) findViewById(R.id.ButtonTUP);
        
        // Initialize the BluetoothChatService to perform bluetooth connections
        mChatService = new BluetoothChatService(this, mHandler);

        // Initialize the buffer for outgoing messages
        mOutStringBuffer = new StringBuffer("");
        
        /*imgView1.setMaxWidth(50); 
        imgView2.setMaxWidth(50);
        imgView3.setMaxWidth(50);
        imgView4.setMaxWidth(50);
        imgView5.setMaxWidth(50);*/
        //imgView5.setMaxWidth(iLinearLayout.getWidth()/5);
        myTimer = new Timer();
        
        myTimer.scheduleAtFixedRate(updateTask, 0, 90L);
        
    }
    
    public float currentXPosition;
    public float currentYPosition;
	private Boolean _dragging = false;
	String MoveType = "Null"; 

    
    @Override
    public boolean onTouchEvent(MotionEvent me) {

        int action = me.getAction();
        currentXPosition = me.getX();
        currentYPosition = me.getY();

        Log.v("MotionEvent", "Action = " + action);
        Log.v("MotionEvent", "X = " + currentXPosition + "Y = " + currentYPosition);

        LayoutAllMove = (TableLayout)findViewById(R.id.tableLayoutRowAll);
        
        LayoutUp= (TableRow)findViewById(R.id.tableRowUP);
        LayoutMidlle= (TableRow)findViewById(R.id.tableRowMidle);
        LayoutDown= (TableRow)findViewById(R.id.tableRowDown);

        MoveType = "Null";
		if ( me.getAction() == MotionEvent.ACTION_DOWN ){
			_dragging = true;
		}else if ( me.getAction() == MotionEvent.ACTION_UP){
			_dragging = false;
			mConversationArrayAdapter.add("MotionEvent.ACTION_UP");
	        //mConversationArrayAdapter.add("L"+mSendButtonRight.getLeft()+"R"+mSendButtonRight.getRight() );
	        //mConversationArrayAdapter.add("B"+mSendButtonRight.getBottom()+ "T"+mSendButtonRight.getTop()  );
	        //mConversationArrayAdapter.add("X"+currentXPosition+"Y"+currentYPosition);
	        
			MoveType = "Null";
			mConversationArrayAdapter.add(MoveType);
	        return true;
		}

		int offset = LayoutMain.getTop()+ LayoutAllMove.getTop();
		
        if ( mSendButtonRight.getLeft()    < currentXPosition && 
           	 mSendButtonRight.getRight()   > currentXPosition &&
           	 offset + LayoutMidlle.getTop()+ mSendButtonRight.getBottom()  > currentYPosition && 
           	 offset + LayoutMidlle.getTop()+ mSendButtonRight.getTop()     < currentYPosition ) {
           	
           	MoveType = "Right";
           	}
           if ( mSendButtonLeft.getLeft()    < currentXPosition && 
              	 mSendButtonLeft.getRight()   > currentXPosition &&
              	offset + LayoutMidlle.getTop()+ mSendButtonLeft.getBottom()  > currentYPosition && 
              	offset + LayoutMidlle.getTop()+ mSendButtonLeft.getTop()     < currentYPosition ) {
              	
              	MoveType = "Left";
              	}
           if ( mSendButtonUp.getLeft()    < currentXPosition && 
              	 mSendButtonUp.getRight()   > currentXPosition &&
              	offset + LayoutUp.getTop()+mSendButtonUp.getBottom()  > currentYPosition && 
              	offset + LayoutUp.getTop()+mSendButtonUp.getTop()     < currentYPosition ) {
              	
              	MoveType = "UP";
              	}
           if ( mSendButtonDown.getLeft()    < currentXPosition && 
              	 mSendButtonDown.getRight()   > currentXPosition &&
              	offset + LayoutDown.getTop()+mSendButtonDown.getBottom()  > currentYPosition && 
              	offset + LayoutDown.getTop()+mSendButtonDown.getTop()     < currentYPosition ) {
              	
              	MoveType = "Down";
              	}
           
           if ( mSendButtonDwR.getLeft()    < currentXPosition && 
        		mSendButtonDwR.getRight()   > currentXPosition &&
              	offset + LayoutDown.getTop()+ mSendButtonDwR.getBottom()  > currentYPosition && 
              	offset + LayoutDown.getTop()+ mSendButtonDwR.getTop()     < currentYPosition ) {
              	
              	MoveType = "DWRight";
           }
           if ( mSendButtonDwL.getLeft()    < currentXPosition && 
            	mSendButtonDwL.getRight()   > currentXPosition &&
                offset + LayoutDown.getTop()+ mSendButtonDwL.getBottom()  > currentYPosition && 
                offset + LayoutDown.getTop()+ mSendButtonDwL.getTop()     < currentYPosition ) {
                 	
                MoveType = "DWLeft";
           }
           if ( mSendButtonUpR.getLeft()    < currentXPosition && 
           		mSendButtonUpR.getRight()   > currentXPosition &&
                offset + LayoutUp.getTop()+ mSendButtonUpR.getBottom()  > currentYPosition && 
                offset + LayoutUp.getTop()+ mSendButtonUpR.getTop()     < currentYPosition ) {
                    	
                MoveType = "UPRight";
           }
           if ( mSendButtonUpL.getLeft()    < currentXPosition && 
                mSendButtonUpL.getRight()   > currentXPosition &&
                offset + LayoutUp.getTop()+ mSendButtonUpL.getBottom()  > currentYPosition && 
                offset + LayoutUp.getTop()+ mSendButtonUpL.getTop()     < currentYPosition ) {
                       	
                MoveType = "UPLeft";
           }

           if ( mSendButtonUpR.getLeft()    < currentXPosition && 
              		mSendButtonUpR.getRight()   > currentXPosition &&
                   offset + LayoutTrubo.getTop()+ mSendButtonUpR.getBottom()  > currentYPosition && 
                   offset + LayoutTrubo.getTop()+ mSendButtonUpR.getTop()     < currentYPosition ) {
                       	
                   MoveType = "TUPRight";
           }
           if ( mSendButtonTUpL.getLeft()    < currentXPosition && 
                   mSendButtonTUpL.getRight()   > currentXPosition &&
                   offset + LayoutTrubo.getTop()+ mSendButtonTUpL.getBottom()  > currentYPosition && 
                   offset + LayoutTrubo.getTop()+ mSendButtonTUpL.getTop()     < currentYPosition ) {
                          	
                   MoveType = "TUPLeft";
           }
           if ( mSendButtonTUp.getLeft()    < currentXPosition && 
                	 mSendButtonTUp.getRight()   > currentXPosition &&
                	offset + LayoutTrubo.getTop()+mSendButtonTUp.getBottom()  > currentYPosition && 
                	offset + LayoutTrubo.getTop()+mSendButtonTUp.getTop()     < currentYPosition ) {
                	
                	MoveType = "TUP";
                	}
           
        mConversationArrayAdapter.add(MoveType);
        	//mConversationArrayAdapter.add("onTouchEvent"+currentXPosition+currentYPosition);
        
        		
        if (action == MotionEvent.ACTION_MOVE) {
             // do something
        }


        if (action == MotionEvent.ACTION_UP) {
             // do something
        }

        return true;
      }

    

    private TimerTask updateTask = new TimerTask() {
        @Override
        public void run() {
          Log.i(TAG, "Timer task doing work");
      	if (MoveType == "DWLeft") {
      		String message ="0";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	if (MoveType == "DWRight") {
      		String message ="1";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	if (MoveType == "UPLeft") {
      		String message ="2";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	if (MoveType == "UPRight") {
      		String message ="3";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	if (MoveType == "UP") {
      		String message ="c";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	if (MoveType == "Left") {
      		String message ="b";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	if (MoveType == "Down") {
      		String message ="a";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	if (MoveType == "Right") {
      		String message ="d";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}


      	if (MoveType == "TUPLeft") {
      		String message ="x";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	if (MoveType == "TUPRight") {
      		String message ="y";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	if (MoveType == "TUP") {
      		String message ="z";
      		byte[] send = message.getBytes();
      	
      		mChatService.write(send);
      	}
      	
      
        }
      };
      

    @Override
    public synchronized void onPause() {
        super.onPause();
        if(D) Log.e(TAG, "- ON PAUSE -");
    }

    @Override
    public void onStop() {
        super.onStop();
        if(D) Log.e(TAG, "-- ON STOP --");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        // Stop the Bluetooth chat services
        if (mChatService != null) mChatService.stop();
        if(D) Log.e(TAG, "--- ON DESTROY ---");
    }

    private void ensureDiscoverable() {
        if(D) Log.d(TAG, "ensure discoverable");
        if (mBluetoothAdapter.getScanMode() !=
            BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
            Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
            startActivity(discoverableIntent);
        }
    }
int ri=1;
int rakaa=1;

private void imgGoNext() {
	
	/*
	for (int j=1 ; j<ri;j++){
		if (j==1) imgView2.setVisibility(0);//4
		if (j==2) imgView3.setVisibility(0);
		if (j==3) imgView4.setVisibility(0);
		if (j==4) imgView5.setVisibility(0);
		if (j==5) imgView6.setVisibility(0);
		if (j==6) imgView7.setVisibility(0);
		
		if ((j==7 && (rakaa == 2 || rakaa == 4 )) || (j==6 && (rakaa == 1 || rakaa == 3) )){
			//imgView1.setVisibility(4);//4
			imgView2.setVisibility(4);//4
			imgView3.setVisibility(4);//4
			imgView4.setVisibility(4);//4
			imgView5.setVisibility(4);//4
			imgView6.setVisibility(4);//4
			imgView7.setVisibility(4);//4
			ri=1;
			if ( rakaa == 4 ) {
				rakaa =1;
				imgViewNumber1.setVisibility(0);//4
				imgViewNumber2.setVisibility(4);//4
				imgViewNumber3.setVisibility(4);//4
				imgViewNumber4.setVisibility(4);//4
			}
			else {
				
				rakaa++;
				if (rakaa==1) imgViewNumber1.setVisibility(0);//4
				if (rakaa==2) imgViewNumber2.setVisibility(0);
				if (rakaa==3) imgViewNumber3.setVisibility(0);
				if (rakaa==4) imgViewNumber4.setVisibility(0);
			
		}
		
		}
		mOutEditText.setText(" Rak3a N°=" + rakaa + " ph°=" + ri);
	}
	*/
}

    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    private void sendMessage(String message) {
    	//ri ++;
    	//imgGoNext();
        // Check that we're actually connected before trying anything
        if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
            Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            mChatService.write(send);

            // Reset out string buffer to zero and clear the edit text field
            mOutStringBuffer.setLength(0);
            mOutEditText.setText(mOutStringBuffer);
        }
    }

    // The action listener for the EditText widget, to listen for the return key
    private TextView.OnEditorActionListener mWriteListener =
        new TextView.OnEditorActionListener() {
        public boolean onEditorAction(TextView view, int actionId, KeyEvent event) {
            // If the action is a key-up event on the return key, send the message
            if (actionId == EditorInfo.IME_NULL && event.getAction() == KeyEvent.ACTION_UP) {
                String message = view.getText().toString();
                sendMessage(message);
            }
            if(D) Log.i(TAG, "END onEditorAction");
            return true;
        }
    };
    // The Handler that gets information back from the BluetoothChatService
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case MESSAGE_STATE_CHANGE:
                if(D) Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
                switch (msg.arg1) {
                case BluetoothChatService.STATE_CONNECTED:
                    //mTitle.setText(R.string.title_connected_to);
                    //mTitle.append(mConnectedDeviceName);
                    mConversationArrayAdapter.clear();
                    break;
                case BluetoothChatService.STATE_CONNECTING:
                    //mTitle.setText(R.string.title_connecting);
                    break;
                case BluetoothChatService.STATE_LISTEN:
                case BluetoothChatService.STATE_NONE:
                    //mTitle.setText(R.string.title_not_connected);
                    break;
                }
                break;
            case MESSAGE_WRITE:
                byte[] writeBuf = (byte[]) msg.obj;
                // construct a string from the buffer
                String writeMessage = new String(writeBuf);
                mConversationArrayAdapter.add("Me:  " + writeMessage);
                break;
            case MESSAGE_READ:
                byte[] readBuf = (byte[]) msg.obj;
                mConversationArrayAdapter.add("Read : " + readBuf);
                break;
            case MESSAGE_DEVICE_NAME:
                // save the connected device's name
                mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
                Toast.makeText(getApplicationContext(), "Connected to "
                               + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                break;
            case MESSAGE_TOAST:
                Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                               Toast.LENGTH_SHORT).show();
                break;
            }
        }
    };

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(D) Log.d(TAG, "onActivityResult " + resultCode);
        switch (requestCode) {
        case REQUEST_CONNECT_DEVICE:
            // When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                // Get the device MAC address
                String address = data.getExtras()
                                     .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                // Get the BLuetoothDevice object
                BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
                // Attempt to connect to the device
                mChatService.connect(device);
            }
            break;
        case REQUEST_ENABLE_BT:
            // When the request to enable Bluetooth returns
            if (resultCode == Activity.RESULT_OK) {
                // Bluetooth is now enabled, so set up a chat session
                setupChat();
            } else {
                // User did not enable Bluetooth or an error occured
                Log.d(TAG, "BT not enabled");
                Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.scan:
            // Launch the DeviceListActivity to see devices and do scan
            Intent serverIntent = new Intent(this, DeviceListActivity.class);
            startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
            return true;
        case R.id.discoverable:
            // Ensure this device is discoverable by others
            ensureDiscoverable();
            return true;
        }
        return false;
    }

}
