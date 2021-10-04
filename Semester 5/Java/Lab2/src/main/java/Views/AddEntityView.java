package Views;
import Actions.ActionToPerform;
import Entities.BaseEntity;
import Mappers.SqlMapper;
import javax.swing.*;
import java.util.HashMap;
import java.util.UUID;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class AddEntityView<TEntity extends BaseEntity>
{
    private final JFrame Frame;
    private final HashMap<String, JTextField> TextFields;
    private final String[] FieldNames;
    private final SqlMapper<TEntity> EntitySqlMapper;

    private final ExecutorService executorService = Executors.newSingleThreadExecutor();
    private ActionToPerform exitAction;

    public TEntity Result;

    public AddEntityView(SqlMapper<TEntity> entitySqlMapper)
    {
        EntitySqlMapper = entitySqlMapper;

        FieldNames = entitySqlMapper.GetFieldNames();

        Frame = new JFrame();
        Frame.setTitle("Add Entity");
        Frame.setSize(200, 100 + FieldNames.length * 30);
        Frame.setLayout(null);

        TextFields = new HashMap<>();

        int y = 10;
        for (var fieldName : FieldNames)
        {
            if(fieldName == "Id")
            {
                continue;
            }

            var label = new JLabel(fieldName, SwingConstants.RIGHT);
            label.setSize(60, 20);
            label.setLocation(0, y);

            var textField = new JTextField();
            textField.setSize(100, 20);
            textField.setLocation(70, y);

            Frame.add(label);
            Frame.add(textField);

            TextFields.put(fieldName, textField);

            y += 30;
        }

        var saveButton = new JButton("Add");
        saveButton.setSize(100, 20);
        saveButton.setLocation(70, y + 20);
        saveButton.addActionListener(e -> SaveButton_Click());

        Frame.add(saveButton);

        Frame.setVisible(true);
    }

    public void SetExitAction(ActionToPerform actionListener)
    {
        exitAction = actionListener;
    }

    private void SaveButton_Click()
    {
        var hashMap = new HashMap<String, String>();

        for(var fieldName : FieldNames)
        {
            if(fieldName == "Id")
            {
                continue;
            }

            var field = TextFields.get(fieldName);

            hashMap.put(fieldName, field.getText());
        }

        hashMap.put("Id", UUID.randomUUID().toString());

        Result = EntitySqlMapper.CreateEntity(hashMap);

        Frame.setVisible(false);

        executorService.execute(() -> exitAction.PerformAction());
    }
}
