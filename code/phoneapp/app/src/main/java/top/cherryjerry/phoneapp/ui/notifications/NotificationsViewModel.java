package top.cherryjerry.phoneapp.ui.notifications;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class NotificationsViewModel extends ViewModel {


    private final MutableLiveData<String> stateText;

    private final MutableLiveData<String> distanceText;

    private final MutableLiveData<String> settingText;

    public NotificationsViewModel() {
        stateText = new MutableLiveData<>();
        distanceText = new MutableLiveData<>();
        settingText = new MutableLiveData<>();
        stateText.setValue("{This is stateText}");
        distanceText.setValue("{This is distanceText}");
        settingText.setValue("{This is settingText}");

    }

    public MutableLiveData<String> getStateText() {
        return stateText;
    }

    public MutableLiveData<String> getDistanceText() {
        return distanceText;
    }

    public MutableLiveData<String> getSettingText() {
        return settingText;
    }
}