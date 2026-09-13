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
        Answer answer = new Answer();

        try {
            answer.Id = UUID.fromString(sql.getString("Id"));
            answer.Text = sql.getString("Text");
        } catch (SQLException sqlException) {
            sqlException.printStackTrace();
            return null;
        }

        return answer;
    }

    @Override
    public Answer CreateEntity(HashMap<String, String> stringHashMap)
    {
        Answer answer = new Answer();

        answer.Id = UUID.fromString(stringHashMap.get("Id"));
        answer.QuestionId = UUID.fromString(stringHashMap.get("QuestionId"));

        answer.Text = stringHashMap.get("Text");

        return answer;
    }

    @Override
    public ArrayList<NameValuePair> GetFields(Answer answer)
    {
        ArrayList<NameValuePair> result = new ArrayList<>();

        result.add(new NameValuePair("Id", answer.Id.toString()));
        result.add(new NameValuePair("QuestionId", answer.QuestionId.toString()));
        result.add(new NameValuePair("Text", answer.Text));

        return result;
    }

    @Override
    public String[] GetFieldNames()
    {
        return new String[]
                {
                        "Id",
                        "QuestionId",
                        "Text"
                };
    }
}
