package Repositories;

import Entities.Answer;
import Mappers.AnswerMapper;
import Mappers.SqlMapper;

public class AnswerRepository extends BaseRepository<Answer>
{
    public AnswerRepository(String connectionString)
    {
        super(connectionString, "Answers", new AnswerMapper());
    }
}
