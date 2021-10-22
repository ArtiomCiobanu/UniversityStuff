package Mappers;

import Entities.Answer;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.UUID;

public class AnswerMapper implements SqlMapper<Answer>
{
    @Override
    public Answer CreateEntity(ResultSet sql)
    {
        Answer question = new Answer();

        try {
            question.Id = UUID.fromString(sql.getString("Id"));
            question.AnswerText = sql.getString("AnswerText");
        } catch (SQLException sqlException) {
            sqlException.printStackTrace();
            return null;
        }

        return question;
    }

    @Override
    public Answer CreateEntity(HashMap<String, String> stringHashMap)
    {
        return null;
    }

    @Override
    public ArrayList<NameValuePair> GetFields(Answer entity)
    {
        return null;
    }

    @Override
    public String[] GetFieldNames()
    {
        return new String[0];
    }
}
