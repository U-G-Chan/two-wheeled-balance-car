package top.cherryjerry.phoneapp.ui.dashboard;

import android.os.Bundle;
import android.text.Editable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.google.android.material.textfield.TextInputEditText;

import java.util.Objects;

import de.kai_morich.simple_bluetooth_terminal.DevicesFragment;
import top.cherryjerry.phoneapp.MsgCode;
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

            if (savedInstanceState == null) {
                getChildFragmentManager().beginTransaction().add(R.id.fragment, new DevicesFragment(), "devices").commit();
            } else {
                Objects.requireNonNull(actionBar).
                        setDisplayHomeAsUpEnabled(getChildFragmentManager().getBackStackEntryCount() > 0);
            }
        }

    }

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        DashboardViewModel dashboardViewModel =
                new ViewModelProvider(this).get(DashboardViewModel.class);

        binding = FragmentDashboardBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        final Button button_BT_connect = root.findViewById(R.id.button_bt_connect);
        button_BT_connect.setOnClickListener(view -> dashboardViewModel.btConnectTest());

        //直立环参数设置
        final Button button_submit_pid_stand = root.findViewById(R.id.button_submit_pid_stand);
        final TextInputEditText edit_stand_Kp = root.findViewById(R.id.edit_stand_Kp);
        final TextInputEditText edit_stand_Ki = root.findViewById(R.id.edit_stand_Ki);
        final TextInputEditText edit_stand_Kd = root.findViewById(R.id.edit_stand_Kd);
        button_submit_pid_stand.setOnClickListener(view -> {
            Editable kp = edit_stand_Kp.getText();
            Editable ki = edit_stand_Ki.getText();
            Editable kd = edit_stand_Kd.getText();
            dashboardViewModel.submitPIDData(MsgCode.SET_PID_STAND,kp,ki,kd);

        });
        //速度环参数设置
        final Button button_submit_pid_velocity = root.findViewById(R.id.button_submit_pid_velocity);
        final TextInputEditText edit_velocity_Kp = root.findViewById(R.id.edit_velocity_Kp);
        final TextInputEditText edit_velocity_Ki = root.findViewById(R.id.edit_velocity_Ki);
        final TextInputEditText edit_velocity_Kd = root.findViewById(R.id.edit_velocity_Kd);
        button_submit_pid_velocity.setOnClickListener(view -> {
            Editable kp = edit_velocity_Kp.getText();
            Editable ki = edit_velocity_Ki.getText();
            Editable kd = edit_velocity_Kd.getText();
            dashboardViewModel.submitPIDData(MsgCode.SET_PID_VELOCITY,kp,ki,kd);

        });
        //转向环参数设置
        final Button button_submit_pid_turn = root.findViewById(R.id.button_submit_pid_turn);
        final TextInputEditText edit_turn_Kp = root.findViewById(R.id.edit_turn_Kp);
        final TextInputEditText edit_turn_Ki = root.findViewById(R.id.edit_turn_Ki);
        final TextInputEditText edit_turn_Kd = root.findViewById(R.id.edit_turn_Kd);
        button_submit_pid_turn.setOnClickListener(view -> {
            Editable kp = edit_turn_Kp.getText();
            Editable ki = edit_turn_Ki.getText();
            Editable kd = edit_turn_Kd.getText();
            dashboardViewModel.submitPIDData(MsgCode.SET_PID_TURN,kp,ki,kd);

        });
        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }




}