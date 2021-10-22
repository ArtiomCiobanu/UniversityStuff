package Repositories;

import Entities.Answer;
import Mappers.SqlMapper;

public class AnswerRepository extends BaseRepository<Answer>
{
    public AnswerRepository(String connectionString, String tableName, SqlMapper<Answer> sqlMapper)
    {
        super(connectionString, tableName, sqlMapper);
    }
}
