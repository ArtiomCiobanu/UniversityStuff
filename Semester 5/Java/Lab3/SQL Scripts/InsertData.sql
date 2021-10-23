declare @q1Id uniqueidentifier = newid()
declare @q2Id uniqueidentifier = newid()
declare @q3Id uniqueidentifier = newid()
declare @q4Id uniqueidentifier = newid()

delete from Questions
insert into Questions values 
(@q1Id, 'question1'),
(@q2Id, 'question2'),
(@q3Id, 'question3'),
(@q4Id, 'question4')

delete from Answers
insert into Answers values
(newid(), @q1Id, N'Юпитер'),
(newid(), @q2Id, N'Земля'),
(newid(), @q3Id, N'Бетельгейзе'),
(newid(), @q4Id, '0')

select * from Questions
select * from Answers
