package top.cherryjerry.phoneapp.ui.notifications;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import top.cherryjerry.phoneapp.databinding.FragmentNotificationsBinding;

public class NotificationsFragment extends Fragment {

    private FragmentNotificationsBinding binding;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        NotificationsViewModel notificationsViewModel =
                new ViewModelProvider(this).get(NotificationsViewModel.class);

        binding = FragmentNotificationsBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        final TextView stateTextView = binding.textListenState;
        notificationsViewModel.getStateText().observe(getViewLifecycleOwner(), stateTextView::setText);
        final TextView diatanceTextView = binding.textListenDistance;
        notificationsViewModel.getDistanceText().observe(getViewLifecycleOwner(), diatanceTextView::setText);
        final TextView settingTextView = binding.textUpdateSetting;
        notificationsViewModel.getSettingText().observe(getViewLifecycleOwner(), settingTextView::setText);

        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}