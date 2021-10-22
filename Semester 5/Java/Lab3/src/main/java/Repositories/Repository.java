package Repositories;

import Entities.BaseEntity;

import java.util.ArrayList;
import java.util.UUID;

public interface Repository<TEntity extends BaseEntity>
{
    void Create(TEntity entity);

    TEntity Read(UUID id);

    ArrayList<TEntity> ReadTop(int amount, int offset);

    void Update(TEntity entity);

    void Delete(UUID id);

    boolean ExistsWithId(UUID id);
}