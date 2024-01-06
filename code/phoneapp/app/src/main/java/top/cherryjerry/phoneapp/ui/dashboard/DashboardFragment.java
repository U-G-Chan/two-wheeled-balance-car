package top.cherryjerry.phoneapp.ui.dashboard;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.lifecycle.ViewModelProvider;

import java.util.Objects;

import de.kai_morich.simple_bluetooth_terminal.DevicesFragment;
import top.cherryjerry.phoneapp.R;
import top.cherryjerry.phoneapp.databinding.FragmentDashboardBinding;


public class DashboardFragment extends Fragment {

    private FragmentDashboardBinding binding;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //bluetooth
        AppCompatActivity activity = (AppCompatActivity) getActivity();
        if (activity != null) {
            ActionBar actionBar = activity.getSupportActionBar();//获取actionBar

            if (savedInstanceState == null)
                getChildFragmentManager().beginTransaction().add(R.id.fragment, new DevicesFragment(), "devices").commit();
            else
                Objects.requireNonNull(actionBar).
                        setDisplayHomeAsUpEnabled(getChildFragmentManager().getBackStackEntryCount()>0);
        }
    }

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        DashboardViewModel dashboardViewModel =
                new ViewModelProvider(this).get(DashboardViewModel.class);

        binding = FragmentDashboardBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        final Button button_BT_connect = root.findViewById(R.id.button_BT_connect);
        button_BT_connect.setOnClickListener(view -> dashboardViewModel.btConnectTest());

        //final TextView textView = binding.textDashboard;
        //dashboardViewModel.getText().observe(getViewLifecycleOwner(), textView::setText);
        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }




}