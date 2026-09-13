<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8"/>
    <title>Lab3 N1</title>

    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css"
          integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2" crossorigin="anonymous"/>
</head>
<!--box-shadow: 1px 1px 15px 2px black; -->

<body class="bg-light">

<div class="container">
    <div class="row justify-content-center">
        <div class="card">
            <h5 class="card-title py-1 text-center"> Ответьте на следующие вопросы: </h5>
            <div class="card-body">
                <form method="POST" action="question">
                    <div class="form-group">
                        <label>Какая планета солнечной системы является самой большой?</label>
                        <input name="question1" type="text" class="form-control" id="question1-input"/>
                    </div>

                    <div class="form-group">
                        <label for="data-select">
                            Какая планета является третьей в солнечной системе?
                        </label>
                        <select name="question2" class="form-control" id="data-select">
                            <option value="Меркурий">Меркурий</option>
                            <option value="Сатурн">Сатурн</option>
                            <option value="Земля">Земля</option>
                            <option value="Марс">Марс</option>
                        </select>
                    </div>


                    <div class="form-group">
                        <label>Какая из планет может взроваться в близжайшее время?</label>

                        <div class="form-check">
                            <label class="form-check-label">
                                <input type="radio" class="form-check-input" value="Солнце" name="question3"/>
                                Солнце
                            </label>
                        </div>

                        <div class="form-check">
                            <label class="form-check-label">
                                <input type="radio" class="form-check-input" value="Бетельгейзе"
                                       name="question3"/>
                                Бетельгейзе
                            </label>
                        </div>

                        <div class="form-check">
                            <label class="form-check-label">
                                <input type="radio" class="form-check-input" value="Сириус" name="question3"/>
                                Сириус
                            </label>
                        </div>
                    </div>

                    <div class="form-group">
                        <label>Является ли Плутон планетой?</label>
                        <div class="form-check">
                            <label class="form-check-label">
                                <input value="1" type="radio" class="form-check-input" name="question4"/>
                                True
                            </label>
                        </div>
                        <div class="form-check">
                            <label class="form-check-label">
                                <input value="0" type="radio" class="form-check-input" name="question4"/>
                                False
                            </label>
                        </div>
                    </div>
                    <a href="Lab4.jsp" style="text-align: left">Lab4</a>
                    <button type="submit" class="btn float-right"
                            style="background-color: darkcyan; color: white;">Отправить
                    </button>
                </form>
            </div>
        </div>
    </div>
</div>
</body>

</html>