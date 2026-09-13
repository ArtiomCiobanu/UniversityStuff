package Entities;

import java.util.Date;
import java.util.UUID;

public class Client extends BaseEntity
{
    public String Name;
    public Date RegistrationDate;

    public UUID GymPassId;
    public UUID ManagerId;
}
