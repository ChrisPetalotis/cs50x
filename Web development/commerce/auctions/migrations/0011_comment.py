# Generated by Django 3.2.4 on 2021-07-09 21:34

import datetime
from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('auctions', '0010_auto_20210710_0034'),
    ]

    operations = [
        migrations.CreateModel(
            name='Comment',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('text', models.CharField(max_length=255)),
                ('time', models.DateTimeField(default=datetime.datetime.now)),
                ('listing', models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, related_name='comments', to='auctions.listing')),
                ('user', models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, related_name='comments', to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]
