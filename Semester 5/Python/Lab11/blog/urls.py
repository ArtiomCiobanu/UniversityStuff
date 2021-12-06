from django.urls import path

from . import views

app_name = 'blog'
urlpatterns = [
    # path('', views.post_list, name='post_list'),
    path('', views.PostListView.as_view(), name='post_list'),
    path('<int:year>/<int:month>/<int:day>/<slug:post>/', views.post_detail, name='post_detail'),

    path('polls', views.poll_list, name='poll_list'),
    path('<slug:poll_slug>', views.poll_detail, name='poll_detail')
]
