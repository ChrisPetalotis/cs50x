# Generated by Django 3.2.4 on 2021-07-09 21:34

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('auctions', '0009_alter_listing_creationtime'),
    ]

    operations = [
        migrations.AlterField(
            model_name='listing',
            name='description',
            field=models.CharField(max_length=250),
        ),
        migrations.DeleteModel(
            name='Comment',
        ),
    ]
