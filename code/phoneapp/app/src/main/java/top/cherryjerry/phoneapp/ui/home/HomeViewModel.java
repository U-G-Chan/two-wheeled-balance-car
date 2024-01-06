package top.cherryjerry.phoneapp.ui.home;

import android.widget.ScrollView;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class HomeViewModel extends ViewModel {

    private static MutableLiveData<String> mText;
    private static StringBuilder stringBuilder;
    public static boolean enableMessageFlag = false;
    private static ScrollView scrollView;
    private int mode = 0;
    private int key = 0;

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

    public void useMode1(){
        mode = 1;
        showMessage("useMode1\n");
    }
    public void useMode2(){
        mode = 2;
        showMessage("useMode2\n");
    }
    public void useMode3(){
        mode = 3;
        showMessage("useMode3\n");
    }

    public void keyUP(){
        key = 1;
        showMessage("keyUP\n");
    }
    public void keyDown(){
        key = 2;
        showMessage("keyDown\n");
    }
    public void keyLeft(){
        key = 3;
        showMessage("keyLeft\n");
    }
    public void keyRight(){
        key = 4;
        showMessage("keyRight\n");
    }
    public void keyRelease(){
        key = 0;
        showMessage("keyRelease\n");
    }

}