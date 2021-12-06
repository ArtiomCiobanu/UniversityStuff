from django.urls import path

from . import views

app_name = 'blog'
urlpatterns = [
    path('answer', views.save_answer, name='save_answer'),

    path('', views.PostListView.as_view(), name='post_list'),
    path('<int:year>/<int:month>/<int:day>/<slug:post>/', views.post_detail, name='post_detail'),

    path('polls', views.PollListView.as_view(), name='poll_list'),
    path('<slug:poll_slug>', views.poll_detail, name='poll_detail')
]
