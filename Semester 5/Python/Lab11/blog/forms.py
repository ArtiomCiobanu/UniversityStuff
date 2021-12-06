from django import forms

from .models import Comment, UserAnswer


class CommentForm(forms.ModelForm):
    class Meta:
        model = Comment
        fields = ('name', 'email', 'body')


class UserAnswerForm(forms.ModelForm):
    class Meta:
        model = UserAnswer
        fields = ('email', 'text')
