package Mappers;

import Entities.Question;

import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.HashMap;

public class QuestionMapper implements SqlMapper<Question>
{
    @Override
    public Question CreateEntity(ResultSet sql)
    {
        return null;
    }

    @Override
    public Question CreateEntity(HashMap<String, String> stringHashMap)
    {
        return null;
    }

    @Override
    public ArrayList<NameValuePair> GetFields(Question entity)
    {
        return null;
    }

    @Override
    public String[] GetFieldNames()
    {
        return new String[0];
    }
}
