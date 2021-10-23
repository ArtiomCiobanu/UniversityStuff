package Mappers;

import Entities.Question;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.UUID;

public class QuestionMapper implements SqlMapper<Question>
{
    @Override
    public Question CreateEntity(ResultSet sql)
    {
        Question question = new Question();

        try
        {
            question.Id = UUID.fromString(sql.getString("Id"));
            question.Name = sql.getString("Name");
        }
        catch (SQLException sqlException)
        {
            sqlException.printStackTrace();
            return null;
        }

        return question;
    }

    @Override
    public Question CreateEntity(HashMap<String, String> stringHashMap)
    {
        Question question = new Question();

        question.Id = UUID.fromString(stringHashMap.get("Id"));
        question.Name = stringHashMap.get("Name");

        return question;
    }

    @Override
    public ArrayList<NameValuePair> GetFields(Question question)
    {
        ArrayList<NameValuePair> result = new ArrayList<>();

        result.add(new NameValuePair("Id", question.Id.toString()));
        result.add(new NameValuePair("Name", question.Name));

        return result;
    }

    @Override
    public String[] GetFieldNames()
    {
        return new String[]
                {
                        "Id",
                        "Name"
                };
    }
}
