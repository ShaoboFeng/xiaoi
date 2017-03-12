package com.zachary.shaobo.xiaoai;

import android.Manifest;
import android.content.pm.PackageManager;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Environment;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.zachary.shaobo.xiaoai.utils.RecordPlayer;

import java.io.File;
import java.io.IOException;


/**
 *
 */
public class LifeUsedActivity extends AppCompatActivity {
    MediaRecorder mediaRecorder;
    private Boolean Recording  = Boolean.FALSE;
    final private int PERMISSIONS_RECORD_AUDIO = 1;
    private TextView record;
    private File recordFile;
    private RecordPlayer player;


    private void record(){
        if (Recording) {
            //stop record
            try{
            if (recordFile != null) {
                mediaRecorder.stop();
                mediaRecorder.release();
                mediaRecorder = null;
            }
            }catch (Exception e) {

            }
            record.setText("stop record");
            Recording = Boolean.FALSE;
            return;
        }
        if (!Environment.getExternalStorageState().equals(
                android.os.Environment.MEDIA_MOUNTED))
        {
            Toast.makeText(LifeUsedActivity.this
                    , "SD卡不存在，请插入SD卡！"
                    , Toast.LENGTH_LONG)
                    .show();
            return;
        }
        Recording = Boolean.TRUE;
        //start record
        record.setText( "start record");
        try {
            recordFile = new File(Environment
                    .getExternalStorageDirectory()
                    .getCanonicalFile() + "/kk.amr");
        }catch (IOException e){
            Toast.makeText(LifeUsedActivity.this
                    , "SD卡错误!"
                    , Toast.LENGTH_LONG)
                    .show();
            return;
        }
        mediaRecorder = new MediaRecorder();
        // 判断，若当前文件已存在，则删除
        if (recordFile.exists()) {
            recordFile.delete();
        }
        mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
        mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.DEFAULT);
        mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.DEFAULT);
        mediaRecorder.setOutputFile(recordFile.getAbsolutePath());

        try {
            mediaRecorder.prepare();
            mediaRecorder.start();
        } catch (IllegalStateException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch (requestCode) {
            case PERMISSIONS_RECORD_AUDIO: {
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    record();
                } else {
                    Toast.makeText(LifeUsedActivity.this
                            , "没有权限录音!"
                            , Toast.LENGTH_LONG)
                            .show();
                }
                return;
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        player=new RecordPlayer(this);
        setContentView(R.layout.activity_life_used);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        record = (TextView)findViewById(R.id.record_text);
        final FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            /**
             * @param view
             */
            @Override
            public void onClick(View view) {
                if ((PackageManager.PERMISSION_GRANTED ==
                        ContextCompat.checkSelfPermission(LifeUsedActivity.this,
                                Manifest.permission.RECORD_AUDIO)) &&
                        PackageManager.PERMISSION_GRANTED == ContextCompat.checkSelfPermission(LifeUsedActivity.this,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE) &&
                        PackageManager.PERMISSION_GRANTED == ContextCompat.checkSelfPermission(LifeUsedActivity.this,
                                Manifest.permission.READ_EXTERNAL_STORAGE)
                        ) {
                    record();
                }
                else{
                    ActivityCompat.requestPermissions(LifeUsedActivity.this,
                            new String[]{Manifest.permission.RECORD_AUDIO,
                                    Manifest.permission.WRITE_EXTERNAL_STORAGE,
                                    Manifest.permission.READ_EXTERNAL_STORAGE},
                            PERMISSIONS_RECORD_AUDIO);
                    return;
                }


            }
        });



        final Button play = (Button) findViewById(R.id.record_play);
        play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(recordFile == null) {
                    try{
                    recordFile = new File(Environment
                            .getExternalStorageDirectory()
                            .getCanonicalFile() + "/kk.amr");
                    }catch (IOException e){
                        return;
                    }
                }
                player.playRecordFile(recordFile);
            }

        });
    }

}
