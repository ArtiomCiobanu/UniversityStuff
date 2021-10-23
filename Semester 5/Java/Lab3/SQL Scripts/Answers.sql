
create --drop 
table Answers
(
	Id uniqueidentifier,
	QuestionId uniqueidentifier,
	Text nvarchar(50) collate Cyrillic_General_CI_AS
)
