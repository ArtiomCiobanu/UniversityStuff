package Views;

import Actions.ActionToPerform;
import javax.swing.*;
import java.util.UUID;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class RemoveEntityView
{
    private final JFrame Frame;

    private final JTextField TextField;

    private final ExecutorService executorService = Executors.newSingleThreadExecutor();
    private ActionToPerform exitAction;

    public UUID EntityId;

    public RemoveEntityView()
    {
        Frame = new JFrame();
        Frame.setTitle("Add Entity");
        Frame.setSize(200, 130);
        Frame.setLayout(null);

        var label = new JLabel("id", SwingConstants.RIGHT);
        label.setSize(60, 20);
        label.setLocation(0, 10);

        TextField = new JTextField();
        TextField.setSize(100, 20);
        TextField.setLocation(70, 10);

        Frame.add(label);
        Frame.add(TextField);

        var saveButton = new JButton("Remove");
        saveButton.setSize(100, 20);
        saveButton.setLocation(70, 60);
        saveButton.addActionListener(e -> RemoveButton_Click());

        Frame.add(saveButton);

        Frame.setVisible(true);
    }

    public void SetExitAction(ActionToPerform actionListener)
    {
        exitAction = actionListener;
    }

    private void RemoveButton_Click()
    {
        EntityId = UUID.fromString(TextField.getText());

        Frame.setVisible(false);

        executorService.execute(() -> exitAction.PerformAction());
    }
}
