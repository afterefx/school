/**
 * Copyright (c) 2010, Christopher Carlisle
 * 
 * This work is licensed under the Creative Commons 
 * Attribution-Noncommercial-No Derivative Works 3.0 United States 
 * License. To view a copy of this license, visit 
 * 
 * http://creativecommons.org/licenses/by-nc-nd/3.0/us/ 
 * 
 * or send a letter to Creative Commons, 171 Second Street, 
 * Suite 300, San Francisco, California, 94105, USA.
 */

package com.android.airpic;

import java.net.*;
import java.io.*;

import android.os.*;
import android.util.Log;

public class FileServer 
{
	private static final String TAG = "Airpic.upload: ";

	public static void upload () throws IOException
	{
		String _path;
		
		Log.i(TAG, "making string...........");
		_path = String.format("%s%s", Environment.getExternalStorageDirectory(), "/images/AirPicTest.jpg");
		
		Log.i(TAG, "opening file...........");
		File file=new File(_path.toString());
		
		Socket sock = new Socket("afterpeanuts.com", 13267);
		Log.i(TAG, "Connecting...........");
		
		byte[] buffer = new byte[35000];
		BufferedOutputStream toServer = new BufferedOutputStream(sock.getOutputStream());
		
		int size = 0;
		long start = System.currentTimeMillis();
		System.err.println("UploadSender.send: Opened port "+13267+" to server.");
		BufferedInputStream fIn = null;
		try 
		{
			fIn = new BufferedInputStream(new FileInputStream(file));
			int n = -1;
			while ( (n=fIn.read(buffer)) != -1 ) 
			{
				size += n;
				toServer.write(buffer, 0, n);
				toServer.flush();
			}
		} 
		finally 
		{
			if(fIn!=null) fIn.close();
				sock.close();
		}
		long end = System.currentTimeMillis();
		System.err.println("UploadSender.send: Done! Sent "+size+" bytes in "+(end-start)+" milliseconds");

    }
}

