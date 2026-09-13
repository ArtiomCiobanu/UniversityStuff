create --drop
table GymPasses
(
	Id uniqueidentifier primary key,
	Price int,
	MonthAmount int
);


create --drop
table Managers
(
	Id uniqueidentifier primary key,
	Name nvarchar(50)
);


create --drop
table Clients
(
	Id uniqueidentifier primary key,
	Name nvarchar(50) not null,
	RegistrationDate datetime not null,

	GymPassId uniqueidentifier,
	ManagerId uniqueidentifier,
	
	constraint FK_Clients_GymPass foreign key (GymPassId) references GymPass (Id),
	constraint FK_Clients_Manager foreign key (ManagerId) references Managers (Id)
);
