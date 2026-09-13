package Repositories;

import Entities.Answer;
import Mappers.AnswerMapper;
import Mappers.SqlMapper;

import java.util.UUID;

public class AnswerRepository extends BaseRepository<Answer>
{
    private static final String TableName = "Answers";

    public AnswerRepository(String connectionString)
    {
        super(connectionString, TableName, new AnswerMapper());
    }

    public Answer ReadByQuestion(UUID questionId)
    {
        var sqlQuery = String.format("select * from %s where QuestionId = '%s'", TableName, questionId);

        var queryResult = ExecuteQuery(sqlQuery);

        var entity = queryResult != null ? GetNextEntity(queryResult) : null;

        CloseConnection();

        return entity;
    }
}
