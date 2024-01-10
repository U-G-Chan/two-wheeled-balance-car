package top.cherryjerry.phoneapp.ui.home;

import android.widget.ScrollView;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import top.cherryjerry.phoneapp.BluetoothFragment;
import top.cherryjerry.phoneapp.MsgCode;

public class HomeViewModel extends ViewModel {

    private static MutableLiveData<String> mText;
    private static StringBuilder stringBuilder;
    public static boolean enableMessageFlag = true;
    private  static ScrollView scrollView;

    private static final String speedStep = "10";


    public HomeViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("信息终端");
        stringBuilder = new StringBuilder();
    }
    public LiveData<String> getText() {
        return mText;
    }

    public void setScrollView(ScrollView scrollView){
        this.scrollView = scrollView;
    }

    public static void showMessage(String message) {
        if(enableMessageFlag){
            stringBuilder.append(message);
            mText.setValue(stringBuilder.toString());
            scrollView.fullScroll(ScrollView.FOCUS_DOWN);
        }
    }

    public void toggleEnableMessageFlag() {
        enableMessageFlag = !enableMessageFlag;
    }
    public void cleanMessage(){
        stringBuilder.setLength(0);
        mText.setValue(stringBuilder.toString());
    }

    public void useMode(MsgCode msgCode){
        if (BluetoothFragment.connected == BluetoothFragment.Connected.True) {
            BluetoothFragment.getInstance()
                    .send("{{"+msgCode.getCode()+"}}");
        }
        showMessage(msgCode.toString());
    }

    public void addSpeed(MsgCode msgCode){
        if (BluetoothFragment.connected == BluetoothFragment.Connected.True) {
            BluetoothFragment.getInstance()
                    .send("{{"+msgCode.getCode()+","+speedStep+"}}");
        }
        showMessage(msgCode.toString());
    }

    public void keyRelease(){
        showMessage("keyRelease\n");
    }

}