package Repositories;

import Entities.Question;
import Mappers.SqlMapper;

public class QuestionRepository extends BaseRepository<Question>
{
    public QuestionRepository(String connectionString, String tableName, SqlMapper<Question> sqlMapper)
    {
        super(connectionString, tableName, sqlMapper);
    }
}
