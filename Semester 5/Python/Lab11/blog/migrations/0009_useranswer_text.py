# Generated by Django 3.2.8 on 2021-12-06 14:12

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('blog', '0008_remove_useranswer_text'),
    ]

    operations = [
        migrations.AddField(
            model_name='useranswer',
            name='text',
            field=models.CharField(default='', max_length=50),
        ),
    ]
