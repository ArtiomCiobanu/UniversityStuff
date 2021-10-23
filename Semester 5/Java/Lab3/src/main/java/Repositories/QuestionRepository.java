package Repositories;

import Entities.Question;
import Mappers.QuestionMapper;

public class QuestionRepository extends BaseRepository<Question>
{
    public QuestionRepository(String connectionString)
    {
        super(connectionString, "Questions", new QuestionMapper());
    }
}
