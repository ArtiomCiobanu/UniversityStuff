# Generated by Django 3.2.8 on 2021-12-06 13:42

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('blog', '0007_auto_20211206_1535'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='useranswer',
            name='text',
        ),
    ]
